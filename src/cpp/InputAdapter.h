//
//  InputAdapter.h
//  Shadow Volumes
//
//  Created by msmith on 4/3/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#ifndef __Shadow_Volumes__InputAdapter__
#define __Shadow_Volumes__InputAdapter__

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
