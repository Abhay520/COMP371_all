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
    Eigen::Vector3f difference = ray.getOrigin()- center;
    auto a = ray.getDirection().squaredNorm();
    auto half_b = difference.dot(ray.getDirection());
    auto c = difference.squaredNorm() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    // no intersection
    if (discriminant < 0) return false;
        // at least one intersection
    else t = (float)(-half_b - sqrt(discriminant)) / a;
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

    triangle1.intersect(ray);
    triangle2.intersect(ray);

    if (triangle1.getT() > 0) {
        t = triangle1.getT();return true;
    }
    if (triangle2.getT() > 0) {
        t = triangle2.getT();return true;
    }
    // no intersection in either triangles
    return false;
}

inline bool Triangle::intersect(Ray &ray) {
    // vectors on the triangle
    Eigen::Vector3f ab = p2 - p1;
    Eigen::Vector3f ac = p3 - p1;

    Eigen::Vector3f pvec = ray.getDirection().cross(ac);
    float det = ab.dot(pvec);

    // if the determinant is close to zero it doesn't intersect
    if (det < 0.0000000000001) return false;

    float inverseDet = 1 / det;

    // distance vector from ray start to a
    Eigen::Vector3f tvec = ray.getOrigin() - p1;

    // (barycentric coordinate between a and b)
    // if between 0 and 1 then it exists inside the triangle
    float u = tvec.dot(pvec) * inverseDet;
    if (u > 1 || u < 0) return false;

    Eigen::Vector3f qvec = tvec.cross(ab);

    // (barycentric coordinate between a and c)
    // if between 0 and 1 then it exists inside the triangle
    float v = ray.getDirection().dot(qvec) * inverseDet;
    // if v is less than 0 or u and v sum to greater than one, it is not in the triangle
    if (u + v > 1 || v < 0) return false;

    // it does exist in the triangle, retrieve t
    t = ac.dot(qvec) * inverseDet;
    return true;
}
