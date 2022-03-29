#pragma once
#include "../external/json.hpp"
#include "../external/simpleppm.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "Eigen/Core"
#include "Scene.h"
#include "Color.h"
#include "Geometry.h"

class RayTracer{
public:
    explicit RayTracer(nlohmann::json& j);
    void run();
private:
    Scene scene;
    nlohmann::json& json;
    //static void save_ppm(const std::string &file_name, const std::vector<float> &buffer, uint dimx, uint dimy);
    static Eigen::Vector3f calculateColorChangeUsingPhong(Ray& ray,const Eigen::Vector3f& intersectionPoint, Light* light, Eigen::Vector3f& normal, Geometry* closestGeometry);
};

struct Parser{
    static void parseGeometry(Scene& scene, nlohmann::json& json);
    static void parseLight(Scene& scene, nlohmann::json& json);
    static void parseOutput(Scene& scene, nlohmann::json& json);
};

inline Eigen::Vector3f RayTracer::calculateColorChangeUsingPhong(Ray &ray,const Eigen::Vector3f& intersectionPoint, Light *light, Eigen::Vector3f &normal, Geometry* closestGeometry) {
    if(light->getType() == LightType::POINT){
        auto* pointLight = dynamic_cast<Point*>(light);
        Eigen::Vector3f  vDirection = (ray.getOrigin() - intersectionPoint).normalized();
        Eigen::Vector3f lDirection = (pointLight->getCenter() - intersectionPoint).normalized();
        Eigen::Vector3f hDirection = (lDirection + vDirection).normalized();
        float cosAngleIncidence =  normal.dot(lDirection);
        float blinn = normal.dot(hDirection);
        if(blinn > 1)blinn = 1;
        else if(blinn < 0)blinn = 0;
        float blinnPhong = pow(blinn, closestGeometry->getPc());
        return closestGeometry->getDc().cwiseProduct(light->getId()) * closestGeometry->getKd() * cosAngleIncidence +
            closestGeometry->getSc().cwiseProduct(light->getIs()) * closestGeometry->getKs() * blinnPhong;
    }
    return Eigen::Vector3f{0,0,0};
//    else{
//        std::cout << "Exiting program: Light type not accepted as of right now" << std::endl;
//        exit(1);
//    }
}

