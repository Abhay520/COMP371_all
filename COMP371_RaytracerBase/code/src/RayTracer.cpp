#include "RayTracer.h"

RayTracer::RayTracer(nlohmann::json &j) : json(j) {}

void Parser::parseGeometry(Scene &scene, nlohmann::json &json) {
    for (auto itr = json["geometry"].begin(); itr!= json["geometry"].end(); itr++){
        if(!itr->contains("type")) {
            std::cout<<"Exiting program: geometry should always contain a type!!!"<< std::endl;
            exit(1);
        }
        std::string type = (*itr)["type"].get<std::string>();
        if(!(itr->contains("ka") && itr->contains("kd") && itr->contains("ks"))) {
            std::cout << "Exiting program : geometry should always have ambient, diffuse and specular reflection coefficients (ka, kd and ks) " << std::endl;
            exit(1);
        }
        float ka = (*itr)["ka"].get<float>();
        float kd = (*itr)["kd"].get<float>();
        float ks = (*itr)["ks"].get<float>();
        if(!itr->contains("pc")){
            std::cout << "Exiting program : geometry should contain phong coefficient (pc) " << std::endl;
            exit(1);
        }
        float pc = (*itr)["pc"].get<float>();
        Eigen::Vector3f ac(0,0,0);Eigen::Vector3f dc(0,0,0);Eigen::Vector3f sc(0,0,0);
        if(itr->contains("ac") && itr->contains("dc") && itr->contains("sc")){
            int i = 0;
            for(auto& itr2 : (*itr)["ac"]){
                if(i < 3) ac[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in ac" << std::endl;
            }
            if(i < 3)std::cout << "Too few arguments provided in ac, the rest were assumed to be 0" << std::endl;
            i = 0;
            for(auto& itr2 : (*itr)["dc"]){
                if(i < 3) dc[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in dc" << std::endl;
            }
            if(i < 3) std::cout << "Too few arguments provided in dc, the rest were assumed to be 0" << std::endl;
            i = 0;
            for(auto& itr2 : (*itr)["sc"]){
                if(i < 3) sc[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in sc" << std::endl;
            }
            if(i < 3)std::cout << "Too few arguments provided in sc, the rest were assumed to be 0" << std::endl;
        }
        else{
            std::cout << "Exiting program : geometry should always have ambient, diffuse and specular reflection color (ac, dc and sc) " << std::endl;
            exit(1);
        }
        Eigen::Matrix4f transform;
        //TODO TRANSFORM MATRIX AND INITIALISE GEOMETRY
        if(itr->contains("transform")){
            //setTransform(transform);
        }
        if(type =="sphere"){
            Eigen::Vector3f center(0,0,0);
            int i = 0;
            for (auto & itr2 : (*itr)["centre"]){
                if(i < 3) center[i++] = itr2.get<float>();
                else std::cout<<"Warning: Too many entries in centre"<< std::endl;
            }
            float radius = 1.0;
            if(itr->contains("radius")) radius = (*itr)["radius"].get<float>();
            else std::cout << "Radius of sphere was not given, so it was assumed to be 1.0f" << std::endl;
            auto* sphere = new Sphere(radius, center);
            auto* geometry = (Geometry*) sphere;
            geometry->setGeometryAttributes(ka, kd, ks, pc, ac, dc, sc);
            scene.addObject(geometry);
        }
        else if(type == "rectangle"){
            Eigen::Vector3f p1(0,0,0);Eigen::Vector3f p2(0,0,0);Eigen::Vector3f p3(0,0,0);Eigen::Vector3f p4(0,0,0);
            if(!(itr->contains("p1") && itr->contains("p2") && itr->contains("p3") && itr->contains("p4"))){
                std::cout << "Exiting program: All 4 points of the rectangle must be given" << std::endl;
                exit(1);
            }
            int i = 0;
            for(auto& itr2 : (*itr)["p1"]){
                if(i < 3) p1[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in p1" << std::endl;
            }
            if(i < 3){
                std::cout << "Exiting program : Too few arguments were provided for p1" << std::endl;
                exit(0);
            }
            i = 0;
            for(auto& itr2 : (*itr)["p2"]){
                if(i < 3) p2[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in p1" << std::endl;
            }
            if(i < 3) {
                std::cout << "Exiting program : Too few arguments were provided for p2" << std::endl;
                exit(0);
            }
            i = 0;
            for(auto& itr2 : (*itr)["p3"]){
                if(i < 3) p3[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in p1" << std::endl;
            }
            if(i < 3){
                std::cout << "Exiting program : Too few arguments were provided for p3" << std::endl;
                exit(0);
            }
            i = 0;
            for(auto& itr2 : (*itr)["p4"]){
                if(i < 3) p4[i++] = itr2.get<float>();
                else std::cout << "Warning : Too many entries in p1" << std::endl;
            }
            if(i < 3){
                std::cout << "Exiting program : Too few arguments were provided for p4" << std::endl;
                exit(0);
            }
            auto* rectangle = new Rectangle(p1, p2, p3, p4);
            auto* geometry = (Geometry*) rectangle;
            geometry->setGeometryAttributes(ka, kd, ks, pc, ac, dc, sc);
            scene.addObject(geometry);
        }
    }
}

void Parser::parseLight(Scene &scene, nlohmann::json &json) {
    for (auto itr = json["light"].begin(); itr!= json["light"].end(); itr++){
        if(!itr->contains("type")) {
            std::cout<<"Exiting program: light should always contain a type!!!"<< std::endl;
            exit(1);
        }
        Eigen::Vector3f id(0,0,0);
        Eigen::Vector3f is(0,0,0);
        if(itr->contains("id")){
            int i = 0;
            for(auto& itr2 : (*itr)["id"]){
                if(i<3) id[i++] = itr2.get<float>();
                else std::cout<<"Warning: Too many entries in id"<< std::endl;
            }
            if(i < 3)std::cout << "Warning: Too few arguments were in id : Some values were assumed to be zero" << std::endl;
        }
        else{
            std::cout<<"Exiting program: light should always contain a diffuse intensity (id)"<< std::endl;
            exit(1);
        }
        if(itr->contains("is")){
            int i = 0;
            for(auto& itr2 : (*itr)["is"]){
                if(i<3)is[i++] = itr2.get<float>();
                else std::cout<<"Warning: Too many entries in centre"<< std::endl;
            }
            if(i < 3)std::cout << "Warning: Too few arguments were in is : Some values were assumed to be zero" << std::endl;
        }
        else{
            std::cout<<"Exiting program: light should always contain a specular intensity (is)"<< std::endl;
            exit(1);
        }
        //Creating super class Light
        std::string type = (*itr)["type"].get<std::string>();
        if(type=="point"){
            std::cout<<"Point based light: "<< std::endl;
            Eigen::Vector3f centre(0,0,0);
            int i = 0;
            for (auto & itr2 : (*itr)["centre"]){
                if(i<3){
                    centre[i++] = itr2.get<float>();
                } else {
                    std::cout<<"Warning: Too many entries in light centre"<< std::endl;
                }
            }
            auto* pointLight = new Point(centre);
            auto* light = (Light*) pointLight;
            light->setId(id);
            light->setIs(is);
            scene.addLight(light);
        }
        else if(type == "area"){
            std::cout << "Area based light: " << std::endl;
            Eigen::Vector3f p1, p2, p3, p4;
            int i = 0;
            for (auto & itr2 : (*itr)["p1"]){
                if(i<3){
                    p1[i++] = itr2.get<float>();
                } else {
                    std::cout<<"Warning: Too many entries in light p1"<< std::endl;
                }
            }
            i = 0;
            for (auto & itr2 : (*itr)["p2"]){
                if(i<3){
                    p2[i++] = itr2.get<float>();
                } else {
                    std::cout<<"Warning: Too many entries in light p2"<< std::endl;
                }
            }
            i = 0;
            for (auto & itr2 : (*itr)["p3"]){
                if(i<3){
                    p3[i++] = itr2.get<float>();
                } else {
                    std::cout<<"Warning: Too many entries in light p3"<< std::endl;
                }
            }
            i = 0;
            for (auto & itr2 : (*itr)["p4"]){
                if(i<3){
                    p4[i++] = itr2.get<float>();
                } else {
                    std::cout<<"Warning: Too many entries in light p4"<< std::endl;
                }
            }
            auto* areaLight = new Area(p1, p2, p3, p4);
            auto* light = (Light*) areaLight;
            light->setId(id);
            light->setIs(is);
            scene.addLight(light);
        }
    }
}

void Parser::parseOutput(Scene &scene, nlohmann::json &json) {
    for(auto itr = json["output"].begin(); itr!= json["output"].end(); itr++){
        if(!itr->contains("filename")){
            std::cout << "Exiting program: Output file not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("size")){
            std::cout << "Exiting program: Output size not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("fov")){
            std::cout << "Exiting program: Output fov not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("up")){
            std::cout << "Exiting program: Output up not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("lookat")){
            std::cout << "Exiting program: Output lookat not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("ai")){
            std::cout << "Exiting program: Output ai not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("bkc")){
            std::cout << "Exiting program: Output bkc not specified" << std::endl;
            exit(1);
        }
        else if(!itr->contains("centre")){
            std::cout << "Exiting program: Output centre not specified" << std::endl;
            exit(1);
        }
        std::string fileName = (*itr)["filename"].get<std::string>();
        int i = 0;
        std::array<uint, 2> size{};
        for(auto& itr2 : (*itr)["size"]){
            if(i < 2)size[i++] = itr2.get<uint>();
            else{
                std::cout << "Excess values in output size ignored" << std::endl;
            }
        }
        if(i != 2){
            std::cout << "Exiting program: Too few arguments in output size" << std::endl;
            exit(1);
        }
        float fov = (*itr)["fov"].get<float>();
        i = 0;
        Eigen::Vector3f up(0,0,0);
        for(auto& itr2 : (*itr)["up"]){
            if(i < 3)up[i++] = itr2.get<float>();
            else std::cout << "Excess values in output up ignored" << std::endl;
        }
        if(i != 3)std::cout << "Warning : Incorrect numer of arguments in output up" << std::endl;
        i = 0;
        Eigen::Vector3f lookAt(0,0,0);
        for(auto& itr2 : (*itr)["lookat"]){
            if(i < 3)lookAt[i++] = itr2.get<float>();
            else std::cout << "Excess values in output lookAt ignored" << std::endl;
        }
        if(i != 3)std::cout << "Warning : Incorrect number of arguments in output lookAt" << std::endl;
        i = 0;
        Eigen::Vector3f centre(0,0,0);
        for(auto& itr2 : (*itr)["centre"]){
            if(i < 3)centre[i++] = itr2.get<float>();
            else std::cout << "Excess values in output centre ignored" << std::endl;
        }
        if(i != 3)std::cout << "Warning : Incorrect number of arguments in output centre" << std::endl;
        i = 0;
        Eigen::Vector3f ai(0,0,0);
        for(auto& itr2 : (*itr)["ai"]){
            if(i < 3)ai[i++] = itr2.get<float>();
            else std::cout << "Excess values in output ai ignored" << std::endl;
        }
        if(i != 3)std::cout << "Warning : Incorrect number of arguments in output ai" << std::endl;
        i = 0;
        Eigen::Vector3f bkc(0,0,0);
        for(auto& itr2 : (*itr)["bkc"]){
            if(i < 3)bkc[i++] = itr2.get<float>();
            else std::cout << "Excess values in output bkc ignored" << std::endl;
        }
        if(i != 3)std::cout << "Warning : Incorrect number of arguments in output bkc" << std::endl;
        auto* output = new Output(fileName, size, fov, up, lookAt, ai, bkc, centre);
        i = 0;
        if(itr->contains("raysperpixel")){
            std::vector<uint> raysPerPixel;
            for(auto & itr2 : (*itr)["raysperpixel"]){
                if(i < 2)raysPerPixel.push_back(itr2.get<uint>());
                else std::cout << "Excess values in output rays per pixel ignored" << std::endl;
            }
            output->setRaysPerPixel(raysPerPixel);
        }
        if(itr->contains("speedup")){
            uint speedUp = (*itr)["speedup"].get<uint>();
            if(speedUp > 1){
                std::cout << "Exiting program: output Speedup value greater than 1" << std::endl;
                exit(1);
            }
            output->setSpeedUp(speedUp);
        }
        if(itr->contains("antialiasing")){
            bool antiAliasing = (*itr)["antialiasing"].get<bool>();
            output->setAntialiasing(antiAliasing);
        }
        if(itr->contains("twosiderender")){
            bool twoSideRender = (*itr)["twosiderender"].get<bool>();
            output->setTwoSideRender(twoSideRender);
        }
        if(itr->contains("globalillum")){
            bool globalillum = (*itr)["globalillum"].get<bool>();
            output->setGlobalIllum(globalillum);
        }
        scene.addOutput(output);
    }
}

void RayTracer::run(){
    std::cout << "Loading the scene" << std::endl;
    Parser parser;
    std::cout << "Parsing geometry" << std::endl;
    parser.parseGeometry(scene, json);
    std::cout << "Parsing geometry completed!" << std::endl;
    std::cout << "Parsing light" << std::endl;
    parser.parseLight(scene, json);
    std::cout << "Parsing light completed!" << std::endl;
    std::cout << "Parsing output" << std::endl;
    parser.parseOutput(scene, json);
    std::cout << "Parsing output completed!" << std::endl;

    std::cout << "Generating image...." << std::endl;
    //For each output
    for(auto output : scene.getOutput()){
        //Image size
        const uint imgWidth = output->getSize()[0];
        const uint imgHeight = output->getSize()[1];
        //Buffer that holds image
        std::vector<double> buffer(3 * imgWidth * imgHeight);
        std::string fileName = output->getFileName();
        //Pixel color
        Color color(0.0,0.0,0.0);
        const auto origin = output->getCenter();
        //Normalising lookAt, up and rightVec
        const auto lookAt = output->getLookAt().normalized();
        const auto up = output->getUp().normalized();
        const auto rightVec = lookAt.cross(up).normalized();
        const float PI = M_PI;
        const float fov = output->getFOV() * PI/180;
        const auto A = origin + lookAt;
        const auto B = A + tan(fov/2) * up;
        const float delta = 2 * tan(fov/2) / imgHeight;
        auto C = B - ((imgWidth/2) * delta * rightVec);
        for(auto it: scene.getSceneObjects()){
            if(it == nullptr){
                std::cout << "NULL" << std::endl;
            }
        }
        for(int j = 0; j < imgHeight;j++){
            for(int i = 0; i < imgWidth; i++) {
                //Pixel we are looking at
                Eigen::Vector3f pixel = C + (j * delta + delta / 2) * rightVec - (i * delta + delta / 2) * up;
                Ray ray(origin, pixel - origin);
                bool intersected = false;
                //Current value of t is infinity
                float closestT = std::numeric_limits<float>::infinity();
                int closestGeometryPosition = -1;
                for (int k = 0; k < scene.getSceneObjects().size(); k++) {
                    auto* geometry = scene.getSceneObjects().at(k);
                    if (geometry->intersect(ray) && geometry->getT() < closestT) {
                        intersected = true;
                        //updating t to its smallest value
                        closestT = geometry->getT();
                        closestGeometryPosition = k;
                    }
                }
                //Determining color of pixel
                if (intersected) {
                    auto* closestGeometry = scene.getSceneObjects().at(closestGeometryPosition);
                    //Finding normal of closest geometry
                    Eigen::Vector3f outwardNormal;
                    if(closestGeometry->getType() == Type::SPHERE){
                        auto* sphere = dynamic_cast<Sphere*>(closestGeometry);
                        outwardNormal = (ray.at(closestT) - sphere->getCenter()).normalized();
                    }
                    else if(closestGeometry->getType() == Type::RECTANGLE){
                        auto* rectangle = dynamic_cast<Rectangle*>(closestGeometry);
                        outwardNormal = ((rectangle->getP2() - rectangle->getP1()).cross(rectangle->getP3() - rectangle->getP1())).normalized();
                    }
                    else{
                        std::cout << "Exiting program: Error at line 396 RayTracer.h" << std::endl;
                        exit(1);
                    }
                    //Reversing normal if its not facing away from the ray
                    auto normal = (ray.getDirection().dot(outwardNormal) < 0)? outwardNormal : -outwardNormal;
                    //Ambient light
                    Eigen::Vector3f colorVector = closestGeometry->getAc().cwiseProduct(output->getAI()) * closestGeometry->getKa();
                    color = Color(colorVector);
                    //Blinn-Phong light calculation

                    for(auto* light: scene.getSceneLights()){
                        Eigen::Vector3f newColorVector = color.getColorVector() + calculateColorChangeUsingPhong(ray, closestT, light, normal, closestGeometry);
                        color = Color(newColorVector);
                    }
                }
                //If ray does not intersect, pixel colour = background colour
                else color = Color(output->getBKC());
                //Update buffer
                color.write(buffer, 3 * i * imgHeight + 3 * j);
            }
        }
        //Saving image to ppm file
        std::cout << "Saving image to ppm file" << std::endl;
        save_ppm(fileName, buffer, imgWidth, imgHeight);
    }
}








































