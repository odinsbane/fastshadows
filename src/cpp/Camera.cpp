#include "Camera.h"


Camera::Camera(GLuint &program, GLuint &shad){
    theProgram = program;
    shadowProgram = shad;
    aspect = 1.0f;
    
	location[0] = 0;
	location[1] = 0;
	location[2] = 0;
	
	position[0] = 0;
	position[1] = 0;
	position[2] = 5;
	
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	
	light_position = glm::vec3(-0.0,-0.0,5.0);
    ambient = glm::vec4(1.0, 1.0, 1.0, 1.0);
    direct = glm::vec4(1.0, 1.0, 1.0, 1.0);
    setViewMatrices();
	
    printf("initializing lights\n");    

    updateLights();
    
}
    
void Camera::zoom(double dr){
    glm::vec3 dx = position - location;
    float length = glm::length(dx);
    if(length + dr>0){
    	length = (length + dr)/length;
    } else{
    	length = 0.5;
    }
    position = location + dx*length;
    setViewMatrices();
}

bool Camera::setFrameBufferTarget(){
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    framebufferName = 0;
    glGenFramebuffers(1, &framebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
    printf("texture buffer @ %d\n", framebufferName);
    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    
    //glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    
    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    setViewMatrices();
    return true;
}

void Camera::setViewMatrices(){
    float fFrustumScale = 1.0f; 
    float fzNear = 0.5f;
    
    float fzFar = 30.0f;
    
		
    GLuint perspectiveMatrixUnif = glGetUniformLocation(theProgram, "perspectiveMatrix");
    
    perspectiveMatrix = glm::perspective(90.0f, aspect, fzNear, fzFar);
    orientationMatrix = glm::lookAt( position, location, up );
    
    perspectiveMatrix = perspectiveMatrix*orientationMatrix;
    
    glUseProgram(theProgram);
    
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, &perspectiveMatrix[0][0]);
    
    glUseProgram(0);
    
}

void Camera::rotate(float dtheta, float dphi){
	
	glm::vec4 nloc = glm::inverse(orientationMatrix)*glm::vec4(dphi, dtheta, 0.0f, 0.0f);
	float angle = glm::length(nloc)*5;
	nloc = glm::normalize(nloc);
	
	glm::vec3 old = position - location;
	
	float l = glm::length(old);
	old = glm::normalize(old);
	
	glm::mat4 rotater = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(nloc));
	glm::vec3 dir = glm::vec3(rotater*glm::vec4(old, 0.0f));
	up = glm::vec3(rotater*glm::vec4(up, 0.0f));
	
	//make sure it is orthogonal and 1.
	up = glm::normalize(up - dir*glm::dot(dir, up));
	
	position = location + dir*l;
    setViewMatrices();
}

void Camera::pan(float dx, float dy){
	glm::vec3 nloc = glm::vec3(glm::inverse(orientationMatrix)*glm::vec4(-dx, -dy, 0.0f, 0.0f));
	
	location = nloc + location;
	position = nloc + position;
    setViewMatrices();
}

/*
 * Changes the display aspect ratio, such that more of the image is shown in a 
 * particular direction.
 *  
 */
void Camera::resizeWindow(float w, float h){
    aspect = w/h;
    
}

void Camera::updateDepthMVP(){
    
    glm::vec3 lightInvDir = light_position - location;
    GLuint depthMatrixID = glGetUniformLocation(shadowProgram, "depthMVP");
    // Compute the MVP matrix from the light's point of view
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-5.f,5.f,-5.f,5.f,-1.f,15.f);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUseProgram(shadowProgram);
    glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
    glUseProgram(0);
    glm::mat4 biasMatrix(
                         0.5, 0.0, 0.0, 0.0,
                         0.0, 0.5, 0.0, 0.0,
                         0.0, 0.0, 0.5, 0.0,
                         0.5, 0.5, 0.5, 1.0
                         );
    glm::mat4 biased = biasMatrix*depthMVP;
    GLuint id = glGetUniformLocation(theProgram, "depthBiasMatrix");
    glUseProgram(theProgram);
    glUniformMatrix4fv(id, 1, GL_FALSE, &biased[0][0]);
    glUseProgram(0);
}

void Camera::updateLights(){

    GLuint lightPositionUniform = glGetUniformLocation(theProgram, "lightPos");
    GLuint lightIntensityUniform = glGetUniformLocation(theProgram, "lightIntensity");
    GLuint ambientIntensityUniform = glGetUniformLocation(theProgram, "ambientIntensity");
    
    glUseProgram(theProgram);
    glUniform3fv(lightPositionUniform, 1, &light_position[0]);
    //printf("pos: %d int: %d amb: %d\n", lightPositionUniform, lightIntensityUniform,ambientIntensityUniform);
    glUniform4fv(lightIntensityUniform, 1, &direct[0]);
    glUniform4fv(ambientIntensityUniform, 1, &ambient[0]);
    glUseProgram(0);
    
    updateDepthMVP();

}

void Camera::moveLight(float dx, float dy, float dz) {
    light_position[0] += dx;
    light_position[1] += dy;
    light_position[2] += dz;
    updateLights();
}

void Camera::setLightPosition(float x, float y, float z) {
    light_position[0] = x;
    light_position[1] = y;
    light_position[2] = z;
    updateLights();
}
