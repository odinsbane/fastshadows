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
#include <stdio.h>
#include "Framework.h"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "math.h"

class Camera {

    GLuint theProgram;
    GLuint shadowProgram;
    GLuint framebufferName;
    GLuint depthTexture;
    glm::vec3 position;
    glm::vec3 location;
    glm::vec3 up;
    glm::mat4 orientationMatrix;
    glm::mat4 perspectiveMatrix;
	float aspect;
	glm::vec3 light_position;
    glm::vec3 ambient;
    glm::vec3 direct;
    public:

        Camera(GLuint &program, GLuint &shadowProgram);
        void resizeWindow(float w, float h);
        void rotate(float dtheta, float dphi);
        void setViewMatrices();
        void zoom(double dr);
        void pan(float dx, float dy);
        void updateLights();
        void moveLight(float dx, float dy, float dz);
        void setLightPosition(float x, float y, float z);
        void updateDepthMVP();
        bool setFrameBufferTarget();
        glm::vec3 getLightPosition(){return light_position;}
		glm::vec3 getTarget(){return location;}
    GLuint getFrameBuffer(){return framebufferName;}
    GLuint getShadowProgram(){return shadowProgram;}
    GLuint getTextureID(){return depthTexture;}

};

#endif /* defined(__Shadow_Volumes__Camera__) */
