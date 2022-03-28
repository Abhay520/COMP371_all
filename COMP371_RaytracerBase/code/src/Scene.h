#pragma once
#include "Light.h"
#include "Output.h"
#include <vector>

class Geometry;

class Scene{
private:
    std::vector<Geometry*>& sceneObjects = *new std::vector<Geometry*>();
    std::vector<Light*>& sceneLights = *new std::vector<Light*>();
    std::vector<Output*>& outputList = *new std::vector<Output*>();
public:
    Scene() = default;
    void addObject(Geometry* shape){
        sceneObjects.push_back(shape);
    }
    void addLight(Light* light){
        sceneLights.push_back(light);
    }
    void addOutput(Output* output){
        outputList.push_back(output);
    }
    std::vector<Geometry*>& getSceneObjects(){
        return sceneObjects;
    }
    std::vector<Light*>& getSceneLights(){
        return sceneLights;
    }
    std::vector<Output*>& getOutput(){
        return outputList;
    }
};