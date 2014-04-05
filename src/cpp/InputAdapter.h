//
//  InputAdapter.h
//  Shadow Volumes
//
//  Created by msmith on 4/3/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#ifndef __Shadow_Volumes__InputAdapter__
#define __Shadow_Volumes__InputAdapter__

#ifndef __APPLE__
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_NO_GLU
#include "GLFW/glfw3.h"
#endif

#include "Camera.h"
#include <iostream>
class InputAdapter{
    Camera* camera;
    GLFWwindow* window;
public:
    InputAdapter(Camera* cam, GLFWwindow* w);
    void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void update();
    int running;
    bool draw_axis;
    
};


#endif /* defined(__Shadow_Volumes__InputAdapter__) */
