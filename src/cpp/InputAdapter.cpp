//
//  InputAdapter.cpp
//  Shadow Volumes
//
//  Created by msmith on 4/3/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#include "InputAdapter.h"

//
//  InputAdapter.cpp
//  CellGL
//
//  Created by msmith on 5/9/13.
//
//

#include "InputAdapter.h"

InputAdapter* main_adapter;

void keyPressedStatic(GLFWwindow* window, int key, int scancode, int action, int mods){
    main_adapter->keyPressed(window, key, scancode, action, mods);
};

InputAdapter::InputAdapter(Camera* cam, GLFWwindow* w){
    camera = cam;
    window = w;
    running = GL_TRUE;
    main_adapter = this;
    glfwSetKeyCallback(w, keyPressedStatic);
    
}

void InputAdapter::keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action==GLFW_PRESS){
        switch(key){
            case GLFW_KEY_ESCAPE:
                running=false;
                break;
        }
    }
}
void InputAdapter::update(){
    // Check if ESC key was pressed or window was closed
    running = (!glfwGetKey(window, GLFW_KEY_ESCAPE )) && !glfwWindowShouldClose(window);
    
    
    if(glfwGetKey(window,  GLFW_KEY_LEFT )){
        camera->rotate(-0.01f, 0);
        
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT)){
        camera->rotate(0.01f, 0);
        
    }
    
    
    if(glfwGetKey(window, GLFW_KEY_UP )){
        camera->rotate(0, 0.01f);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN)){
        camera->rotate(0, -0.01f);
    }
    
    
    if( glfwGetKey(window, GLFW_KEY_A ) ){
        camera->zoom(-0.1f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_Z ) ){
        camera->zoom(0.1f);
    }
    
    
    
    
}

