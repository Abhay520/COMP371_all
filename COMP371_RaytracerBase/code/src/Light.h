#pragma once

#include "Eigen/Core"

enum LightType{LIGHT, POINT, AREA};

class Light{
public:
    Light(Eigen::Vector3f& id, Eigen::Vector3f& is): id(id), is(is){};
    Light() = default;
    //Optional transform matrix
    Eigen::Matrix4f transform;
    virtual std::ostream& operator << (std::ostream& out){
        out << "Light with id : " << id << " is : " << is << std::endl;
    }
    Eigen::Vector3f & getId() { return id; }
    Eigen::Vector3f & getIs(){ return is; }
    virtual LightType getType() const{return LightType::LIGHT;}
    void setId(Eigen::Vector3f& id1){id = id1;}
    void setIs(Eigen::Vector3f& is1){is = is1;}
private:
    //Diffuse and specular light intensity
    Eigen::Vector3f id, is;
};

class Area : public Light{
public:
    Area(Eigen::Vector3f& p1, Eigen::Vector3f& p2, Eigen::Vector3f& p3, Eigen::Vector3f& p4):p1(p1), p2(p2), p3(p3), p4(p4){};
    Eigen::Vector3f& getP1(){return p1;}
    Eigen::Vector3f& getP2(){return p2;}
    Eigen::Vector3f& getP3(){return p3;}
    Eigen::Vector3f& getP4(){return p4;}
    LightType getType() const override{return LightType::AREA;}
private:
    Eigen::Vector3f p1, p2, p3, p4;
};

class Point : public Light{
public:
    Point(Eigen::Vector3f& center): center(center){};
    Eigen::Vector3f& getCenter(){return center;}
    LightType getType() const override {return LightType::POINT;}
private:
    Eigen::Vector3f center;
};