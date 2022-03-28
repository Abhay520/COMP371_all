#pragma once
#include <string>
#include <array>
#include <vector>
#include "Eigen/Core"

class Output{
private:
    //-----Mandatory members of output class----------------------
    //Name of ppm file for file output
    std::string fileName;
    //Resolution of image
    std::array<uint,2> size;
    float fov;
    //Up vector of camera --> Not necessarily unit length
    Eigen::Vector3f up;
    //Lookat vector of camera --> Not necessarily unit length
    Eigen::Vector3f lookAt;
    //Ambient intensity of the scene
    Eigen::Vector3f ai;
    //Background color of output
    Eigen::Vector3f bkc;
    //Center
    Eigen::Vector3f center;
    //------Optional members of output class----------------------
    //Raysperpixel  used for antialiasing
    //If it has only one value --> value represents number of rays per pixel
    //If it has two --> Represents grid dimensions for stratified sampling
    std::vector<uint> raysPerPixel;
    //Speedup --> 0 for no accelerating structure,1 for accelerating structure
    //TODO: Reject values more than 1
    uint speedUp;
    //Antialiasing boolean --> Whether to use if or not
    bool antiAliasing = false;
    //TwoSideRender
    //When true --> Light bounces same way
    bool twoSideRender;
    //Globalillum --> When true, render with global illumination
    bool globalIllum;
public:
    //Constructor of output containing all the mandatory members
    Output(std::string fileName, std::array<uint,2>& size, float fov, Eigen::Vector3f& up, Eigen::Vector3f& lookAt, Eigen::Vector3f& ai, Eigen::Vector3f& bkc, Eigen::Vector3f& center):
    fileName(std::move(fileName)), size(size), fov(fov), up(up), lookAt(lookAt), ai(ai), bkc(bkc), center(center){};
    //Setters only for optional members
    void setRaysPerPixel(std::vector<uint>& rpp){
        raysPerPixel = rpp;
    }
    void setSpeedUp(uint speed){
        speedUp = speed;
    }
    void setAntialiasing(bool aa){
        antiAliasing = aa;
    }
    void setTwoSideRender(bool tsr){
        twoSideRender = tsr;
    }
    void setGlobalIllum(bool global){
        globalIllum = global;
    }
    //Getters for all members
    std::string getFileName(){return fileName;}
    std::array<uint, 2> & getSize() {return size;}
    float getFOV() const{return fov;}
    Eigen::Vector3f& getUp(){return up;}
    Eigen::Vector3f& getLookAt(){return lookAt;}
    Eigen::Vector3f& getAI(){return ai;}
    Eigen::Vector3f& getBKC(){return bkc;}
    Eigen::Vector3f& getCenter(){return center;}
    std::vector<uint>& getRPP(){return raysPerPixel;}
    uint getSpeedUp() const{return speedUp;}
    bool getAntiAliasing() const{return antiAliasing;}
    bool getTwoSideRender()const{return twoSideRender;}
    bool getGlobalIllum()const{return globalIllum;}
};