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
    Color sendRay(Output* output);
    static Eigen::Vector3f calculateColorChangeUsingPhong(Ray& ray,Output* output, const Eigen::Vector3f& intersectionPoint, Light* light, Eigen::Vector3f& normal, Geometry* closestGeometry);
};

struct Parser{
    static void parseGeometry(Scene& scene, nlohmann::json& json);
    static void parseLight(Scene& scene, nlohmann::json& json);
    static void parseOutput(Scene& scene, nlohmann::json& json);
};

inline Eigen::Vector3f RayTracer::calculateColorChangeUsingPhong(Ray &ray,Output* output, const Eigen::Vector3f& intersectionPoint, Light *light, Eigen::Vector3f &normal, Geometry* closestGeometry) {
    if(light->getType() == LightType::POINT){
        auto* pointLight = dynamic_cast<Point*>(light);
        // unit vector representing point of incidence to the camera object
        Eigen::Vector3f vdir = (output->getCenter() - intersectionPoint).normalized();

        // unit vector representing the point of incidence to the light object
        Eigen::Vector3f ldir = (pointLight->getCenter() - intersectionPoint).normalized();

        // 'half-way' vector (unit) halfway between ldir and vdir
        Eigen::Vector3f hdir = (ldir + vdir).normalized();

        // angle NL
        float thetaNL = normal.dot(ldir);
        // light intensity
        float blinn = normal.dot(hdir);

        // clamp value of blinn btw/ 0 and 1
        if (blinn > 1) blinn = 1;
        else if (blinn < 0) blinn = 0;

        // specular phong
        float blinnPhong = pow(blinn, closestGeometry->getPc());

        // diffuse and specular I
        Eigen::Vector3f Idd = light->getId().cwiseProduct(closestGeometry->getDc()) * closestGeometry->getKd() * thetaNL;
        Color colorIDD = Color(Idd);
        Eigen::Vector3f Ids = light->getIs().cwiseProduct(closestGeometry->getSc()) * closestGeometry->getKs()* blinnPhong;
        Color colorIds = Color(Ids);
        // diffuse and specular
        return colorIDD.getColorVector() + colorIds.getColorVector()
    }

    return Eigen::Vector3f{0,0,0};
//    else{
//        std::cout << "Exiting program: Light type not accepted as of right now" << std::endl;
//        exit(1);
//    }
}

