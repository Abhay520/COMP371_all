#pragma once
#include "Eigen/Dense"
#include <vector>

//RGB style Color class
class Color{
private:
    Eigen::Vector3f colorVector;
public:
    Color(){colorVector << 0,0,0;}
    Color(float red, float green, float blue){
        if(red < 0)red = 0;
        if(red > 1)red = 1;
        if(green < 0)green = 0;
        if(green > 1)green = 1;
        if(blue < 0)blue = 0;
        if(blue > 1)blue = 1;
        colorVector << red, green ,blue;
    }
    explicit Color(Eigen::Vector3f& vector) : Color(vector.x(),vector.y(), vector.z()){};
    float getRed() const{return colorVector.x();}
    float getGreen() const{return colorVector.y();}
    float getBlue() const{return colorVector.z();}
    Eigen::Vector3f& getColorVector(){return colorVector;}
    void setRed(float red){
        if(red < 0)red = 0;
        if(red > 1)red = 1;
        colorVector << red, colorVector.y(), colorVector.z();
    }
    void setGreen(float green){
        if(green < 0)green = 0;
        if(green > 1)green = 1;
        colorVector << colorVector.x(), green , colorVector.z();
    }
    void setBlue(float blue){
        if(blue < 0)blue = 0;
        if(blue > 1)blue = 1;
        colorVector << colorVector.x(), colorVector.y(), blue;
    }
    void set(Color& color){colorVector << color.getRed(), color.getGreen(), color.getBlue();}
    void write(std::vector<double>& buffer, int position){
        if(colorVector.x() < 0){
            buffer[position] = 0;
        }
        else if(colorVector.x() > 1){
            buffer[position] = 1;
        }
        else{
            buffer[position] = colorVector.x();
        }
        if(colorVector.y() < 0){
            buffer[position + 1] = 0;
        }
        else if(colorVector.y() > 1){
            buffer[position + 1] = 1;
        }
        else{
            buffer[position + 1] = colorVector.y();
        }
        if(colorVector.z() < 0){
            buffer[position  + 2] = 0;
        }
        else if(colorVector.z() > 1){
            buffer[position + 2] = 1;
        }
        else{
            buffer[position + 2] = colorVector.z();
        }
    }
};