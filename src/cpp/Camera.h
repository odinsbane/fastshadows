//
//  Camera.h
//  Shadow Volumes
//
//  Created by msmith on 4/3/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#ifndef __Shadow_Volumes__Camera__
#define __Shadow_Volumes__Camera__

#include <iostream>

#ifndef __APPLE__
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_NO_GLU
#include "GLFW/glfw3.h"
#endif

#include "math.h"

class Camera{
    
    GLuint theProgram;
    float perspectiveMatrix[16];
    float orientationMatrix[16];
    float normalMatrix[9];
    
    float quarternion[4];
    float aspect;
    float x,y,z;
    float r;
    void setPosition();
    void setRotation();
    void rotate(float* quat);
    float* light_position;
    float* ambient_light;
    float* light_intensity;
    
public:
    
    Camera(GLuint &program);
    void resizeWindow(float w, float h);
    void rotate(float dtheta, float dphi);
    void setPerspectiveMatrix();
    void zoom(double dr);
    void updatePosition();
    void updateLights();
    
};



#endif /* defined(__Shadow_Volumes__Camera__) */
