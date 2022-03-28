#pragma once
#include <Eigen/Core>
#include "Ray.h"

enum Type{RECTANGLE, TRIANGLE, SPHERE, GEOMETRY};

//Geometry super class
class Geometry{
protected:
    //Ambient, diffuse and specular reflection coefficient of the rectangle
    float ka, kd, ks;
    //Phong coefficient
    float pc{};
    //Ambient, diffuse and specular reflection color of the surface of the rectangle
    Eigen::Vector3f ac{}, dc{}, sc{};
    //Optional transform matrix
    Eigen::Matrix4f transform;
public:
    Geometry() = default;
    Geometry(float ka, float kd, float ks, float pc, Eigen::Vector3f& ac, Eigen::Vector3f& dc, Eigen::Vector3f& sc):
            ka(ka), kd(kd), ks(ks), pc(pc), ac(ac), dc(dc), sc(sc){}
    void setTransform(Eigen::Matrix4f& matrix){transform = matrix;}
    virtual bool intersect(Ray& ray) {return false;}
    virtual Type getType(){return GEOMETRY;}
    virtual float getT()const{return -1.0;}
    float getKa() const{return ka;}
    float getKd() const{return kd;}
    float getKs() const{return ks;}
    float getPc() const{return pc;}
    Eigen::Vector3f& getAc(){return ac;}
    Eigen::Vector3f& getDc(){return dc;}
    Eigen::Vector3f& getSc(){return sc;}
    friend std::ostream& operator << (std::ostream& out, Geometry& geometry){
        return out << "Ka = " << geometry.ka << " kd = " << geometry.kd << " ks = " << geometry.ks << " pc = " << geometry.pc << std::endl;
    }
    void setGeometryAttributes(float ka1, float kd1, float ks1, float pc1, Eigen::Vector3f& ac1, Eigen::Vector3f& dc1, Eigen::Vector3f& sc1){
        ka = ka1;kd = kd1;ks = ks1; pc = pc1;ac = ac1;dc = dc1; sc = sc1;
    }
};

//Sphere child class
class Sphere: public Geometry{
private:
    //Radius of the sphere
    float radius;
    //Center of sphere
    Eigen::Vector3f center;
    //Smallest t at which ray intersects with sphere
    float t = -1;
public:
    //Geometry of the sphere (containing all the mandatory members of a geometry)
    Sphere(float radius, Eigen::Vector3f& center): radius(radius), center(center){};

    float getT() const override{return t;}
    bool intersect(Ray& ray) override;
    Type getType() override{return Type::SPHERE;}
    float getRadius() const{return radius;}
    Eigen::Vector3f& getCenter(){return center;}
    friend std::ostream& operator << (std::ostream& out, Sphere& sphere){
        return out << "Sphere of radius " << sphere.radius << " and centre " << sphere.center << std::endl;
    }
    //Setting Geometry attributes
};

//Getting the smallest value of t
inline bool Sphere::intersect(Ray& ray){
    Eigen::Vector3f AminusC = ray.getOrigin() - center;
    auto h = ray.getDirection().dot(AminusC);
    auto a = ray.getDirection().dot(ray.getDirection());
    auto c = AminusC.dot(AminusC) - radius*radius;
    auto discriminant = h * h - a*c;
    if(discriminant < 0)return false;
    float t1 = (-h + sqrt(discriminant))/a;
    float t2 = (-h - sqrt(discriminant))/a;
    if(t1 > t2)std::swap(t1,t2);
    if(t1 < 0){
        t1 = t2; // if t1 is negative, using t2 instead
        //If both are negative, the ray does not intersect sphere
        if(t1 < 0)return false;
    }
    //Assign sphere's t to smallest t t1
    t = t1;
    return true;
}

class Rectangle : public Geometry{
private:
    //4 Corners of the Rectangle
    Eigen::Vector3f p1, p2, p3, p4;
    //Optional transform matrix
    Eigen::Matrix4f transform;
    //Smallest t of intersection
    float t = -1;
public:
    //type (which will be always be RECTANGLE)
    Rectangle(Eigen::Vector3f& p1, Eigen::Vector3f& p2, Eigen::Vector3f& p3 ,Eigen::Vector3f& p4): p1(p1), p2(p2), p3(p3), p4(p4){};
    bool intersect(Ray& ray) override;
    Type getType() override{return Type::RECTANGLE;}
    float getT() const override{return t;}
    Eigen::Vector3f& getP1(){return p1;}
    Eigen::Vector3f& getP2(){return p2;}
    Eigen::Vector3f& getP3(){return p3;}
    Eigen::Vector3f& getP4(){return p4;}
    friend std::ostream& operator << (std::ostream& out, Rectangle& rectangle){
        return out << "Rectangle of points p1 " << rectangle.p1 << ", p2 " << rectangle.p2 << ", p3 " << rectangle.p3 << " and p4 " << rectangle.p4;
    }
};

//Triangle used in intersection of rectangles
struct Triangle : Geometry{
private:
    //Smallest t of intersection
    float t = -1;
    Eigen::Vector3f p1,p2,p3;
public:
    bool intersect(Ray& ray) override;
    Triangle(Eigen::Vector3f& p1, Eigen::Vector3f& p2, Eigen::Vector3f& p3): p1(p1), p2(p2), p3(p3){};
    float getT() const override{return t;}
    Type getType() override{return Type::TRIANGLE;}
};

inline bool Rectangle::intersect(Ray &ray) {
    //Breaking the rectangle into two triangles
    Triangle triangle1(p1, p2, p3);
    Triangle triangle2(p1, p3, p4);
    //If intersect is true, get smallest value of t
    float t1;
    bool triangle1Intersect = triangle1.intersect(ray);
    bool triangle2Intersect = triangle2.intersect(ray);
    //If only triangle1 intersect or both
    if(triangle1Intersect){
        t1 = triangle1.getT();
        if(triangle2Intersect){
            if(triangle2.getT() < t1)t1 = triangle2.getT();
        }
        t = t1;
        return true;
    }
        //If only triangle2 intersect
    else if(triangle2Intersect){
        t = triangle2.getT();
        return true;
    }
        //If no intersection
    else return false;
}

inline bool Triangle::intersect(Ray &ray) {
    //R(t) = P + td where P is the ray origin and d is the ray direction
    //n . R(t) = d
    //replacing R(t) by its equation and making t subject of formula
    //we get t =  (d - n . P)/(n . d)
    //n is the normalized cross product
    auto n = (p2 - p1).cross(p3 - p1);
    //to normalise n , we divide by its magnitude
    n = n/n.norm();
    //Now to find d, we simply multiply by any point on the triangle
    auto d = n.dot(p1);
    //now to find t
    auto nDotD = n.dot(ray.getDirection());
    if(nDotD == 0){
        //Does not intersect triangle
        return false;
    }
    auto t1 = (d - n.dot(ray.getOrigin()))/(nDotD);
    if(t1 < 0){
        //Does not intersect triangle
        return false;
    }
    //Now testing if point of intersection is actually inside the triangle
    //For this to be true, cond1, cond2, and cond3 should both be greater than zero (barycentric coordinates)
    //Here q is the point of intersection
    auto q = ray.at(t1);
    auto cond1 = ((p2 - p1).cross(q - p1)).dot(n);
    auto cond2 = ((p3 - p2).cross(q - p2)).dot(n);
    auto cond3 = ((p1 - p3).cross(q - p3)).dot(n);
    if(cond1 >=0 && cond2 >= 0 && cond3 >= 0){
        t = t1;return true;
    }
    else return false;
}
