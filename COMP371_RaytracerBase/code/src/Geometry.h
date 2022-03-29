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
    virtual void resetT(){}
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
    void resetT() override{t = -1;}
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
    void resetT() override{t = -1;}
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
    // Edges
    Eigen::Vector3f ab = p2 - p1;
    Eigen::Vector3f ac = p3 - p1;

    Eigen::Vector3f pvec = ray.getDirection().cross(ac);
    float det = ab.dot(pvec);

    // If det is very close to zero, not necessarily at 0, the ray is parallel and there is
    // no intersection or it is negligible
    if (det < 0.000001)
        return false; // No intersection

    float invDet = 1.0 / det;
    Eigen::Vector3f tvec = ray.getOrigin() - p1;

    // "u" is the first barycentric coordinate
    float u = tvec.dot(pvec) * invDet;
    // if it is outside of the 0-1 range, the point lies outside of the triangle
    if (u < 0 || u > 1)
        return false; // No intersection

    Eigen::Vector3f qvec = tvec.cross(ab);

    // "v" is the second barycentric coordinate
    float v = ray.getDirection().dot(qvec) * invDet;
    // if it is outside of the 0-1 range, or u+v > 1, the point lies outside of the triangle
    if (v < 0 || u + v > 1)
        return false; // No intersection

    t = ac.dot(qvec) * invDet;
    return true;
}
