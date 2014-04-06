//
//  Camera.cpp
//  Shadow Volumes
//
//  Created by msmith on 4/3/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#include "Camera.h"

Camera::Camera(GLuint &program){
    theProgram = program;
    aspect = 1.0f;
    setPerspectiveMatrix();
    
    r = 3;
    for(int i = 0; i<4; i++){
        quarternion[i] = 0.0;
    }
    quarternion[3] = 1.0;
    
    updatePosition();
    printf("initializing lights\n");
    light_position = new float[3]{-0.5,-0.5,10.0};
    light_intensity = new float[4]{0.9,0.9,0.9,1};
    ambient_light = new float[4]{0.5, 0.5, 0.5, 1.0};
    updateLights();
    
}

void Camera::setLightPosition(float x, float y, float z){

    light_position[0]=x;
    light_position[1]=y;
    light_position[2]=z;
    updateLights();
}

void Camera::setPosition(){
    
    x = -orientationMatrix[2]*r;
    y = -orientationMatrix[6]*r;
    z = -orientationMatrix[10]*r;
    
    GLuint cam_offset = glGetUniformLocation(theProgram, "cam_offset");
    glUseProgram(theProgram);
    glUniform3f(cam_offset, x, y, z);
    glUseProgram(0);
    
}

void Camera::zoom(double dr){
    
    r += dr;
    
    updatePosition();
    
    
}

void Camera::updatePosition(){
    setRotation();
    setPosition();
}


void Camera::setPerspectiveMatrix(){
    float fFrustumScale = 1.0f;
    float fzNear = 0.5f;
    
    float fzFar = 10.0f;
    
    //column
    perspectiveMatrix[0] = fFrustumScale/aspect;
    perspectiveMatrix[1] = 0;
    perspectiveMatrix[2] = 0;
    perspectiveMatrix[3] = 0;
    
    //column
    perspectiveMatrix[4] = 0;
    perspectiveMatrix[5] = fFrustumScale;
    perspectiveMatrix[6] = 0;
    perspectiveMatrix[7] = 0;
    
    //column
    perspectiveMatrix[8] = 0;
    perspectiveMatrix[9] = 0;
    perspectiveMatrix[10] = (fzFar + fzNear) / (fzNear - fzFar);
    perspectiveMatrix[11] = -1.0f;
    
    //column
    perspectiveMatrix[12] = 0;
    perspectiveMatrix[13] = 0;
    perspectiveMatrix[14] = 2.f*fzFar*fzNear / (fzNear - fzFar);
    perspectiveMatrix[15] = 0;
    
    
    
    GLuint perspectiveMatrixUnif = glGetUniformLocation(theProgram, "perspectiveMatrix");
    
    glUseProgram(theProgram);
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);
    glUseProgram(0);
    
}


void Camera::rotate(float dtheta, float dphi){
    if(dtheta==0){
        float axis[4];
        float sdp = sin(dphi/2.0);
        axis[0] = sdp;
        axis[1] = 0;
        axis[2] = 0;
        axis[3] = cos(dphi/2.0);
        rotate(axis);
    } else{
        
        float axis[4];
        float sdp = sin(dtheta/2.0);
        axis[0] = 0;
        axis[1] = sdp;
        axis[2] = 0;
        axis[3] = cos(dtheta/2.0);
        rotate(axis);
        
    }
    updatePosition();
}

void Camera::rotate(float* quat){
    float next[4];
    float sum = 0;
    next[0] = quarternion[3]*quat[0] + quarternion[0]*quat[3] + quarternion[1]*quat[2] - quarternion[2]*quat[1];
    sum += next[0]*next[0];
    next[1] = quarternion[3]*quat[1] + quarternion[1]*quat[3] + quarternion[2]*quat[0] - quarternion[0]*quat[2];
    sum += next[1]*next[1];
    next[2] = quarternion[3]*quat[2] + quarternion[2]*quat[3] + quarternion[0]*quat[1] - quarternion[1]*quat[0];
    sum += next[2]*next[2];
    next[3] = quarternion[3]*quat[3] - quarternion[0]*quat[0] - quarternion[1]*quat[1] - quarternion[2]*quat[2];
    sum += next[3]*next[3];
    sum = sqrt(sum);
    for(int i = 0; i<4; i++){
        quarternion[i] = next[i]/sum;
    }
    
}

void Camera::setRotation(){
    orientationMatrix[0] = 1 - 2*quarternion[1]*quarternion[1]-2*quarternion[2]*quarternion[2];
    orientationMatrix[1] = 2*quarternion[0]*quarternion[1] + 2*quarternion[3]*quarternion[2];
    orientationMatrix[2] = 2*quarternion[0]*quarternion[2] - 2*quarternion[3]*quarternion[1];
    orientationMatrix[3] = 0;
    
    orientationMatrix[4] = 2*quarternion[0]*quarternion[1] - 2*quarternion[3]*quarternion[2];
    orientationMatrix[5] = 1 - 2*quarternion[0]*quarternion[0]-2*quarternion[2]*quarternion[2];
    orientationMatrix[6] = 2*quarternion[1]*quarternion[2] + 2*quarternion[3]*quarternion[0];
    orientationMatrix[7] = 0;
    
    orientationMatrix[8] = 2*quarternion[0]*quarternion[2] + 2*quarternion[3]*quarternion[1];
    orientationMatrix[9] = 2*quarternion[1]*quarternion[2] - 2*quarternion[3]*quarternion[0];
    orientationMatrix[10] = 1 - 2*quarternion[0]*quarternion[0]-2*quarternion[1]*quarternion[1];
    orientationMatrix[11] = 0;
    
    orientationMatrix[12] = 0;
    orientationMatrix[13] = 0;
    orientationMatrix[14] = 0;
    orientationMatrix[15] = 1;
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            normalMatrix[i + 3*j] = orientationMatrix[i + 4*j];
        }
    }
    GLuint orientationMatrixUnif = glGetUniformLocation(theProgram, "orientationMatrix");
    GLuint normalMatrixUniform = glGetUniformLocation(theProgram, "normalModelToCameraMatrix");
    
    glUseProgram(theProgram);
    
    glUniformMatrix4fv(orientationMatrixUnif, 1, GL_FALSE, orientationMatrix);
    glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, normalMatrix);
    glUseProgram(0);
    
}

/*
 * Changes the display aspect ratio, such that more of the image is shown in a
 * particular direction.
 *
 */
void Camera::resizeWindow(float w, float h){
    aspect = w/h;
    
}

void Camera::updateLights(){

    GLuint lightPositionUniform = glGetUniformLocation(theProgram, "lightPos");
    GLuint lightIntensityUniform = glGetUniformLocation(theProgram, "lightIntensity");
    GLuint ambientIntensityUniform = glGetUniformLocation(theProgram, "ambientIntensity");
    glUseProgram(theProgram);
    
    
    glUniform3fv(lightPositionUniform, 1, light_position);
    glUniform4fv(lightIntensityUniform, 1, light_intensity);
    
    glUniform4fv(ambientIntensityUniform, 1, ambient_light);

    glUseProgram(0);
    
    
}

