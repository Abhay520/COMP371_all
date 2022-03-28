#pragma once
#include <Eigen/Core>
#include <utility>

class Ray{
private:
    Eigen::Vector3f rayOrigin;
    Eigen::Vector3f rayDirection;
public:
    Ray(Eigen::Vector3f  origin, Eigen::Vector3f  direction): rayOrigin(std::move(origin)), rayDirection(std::move(direction)){};
    Eigen::Vector3f& getOrigin(){return rayOrigin;}
    Eigen::Vector3f& getDirection(){return rayDirection;}
    //P(t) = A + tb where A is the ray origin and b is the ray direction
    Eigen::Vector3f at(float t){
        return rayOrigin + t * rayDirection;
    }
};