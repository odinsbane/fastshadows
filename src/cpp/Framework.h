//
//  Framework.h
//  Shadow Volumes
//
//  Created by msmith on 4/4/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#ifndef __Shadow_Volumes__Framework__
#define __Shadow_Volumes__Framework__

#include <iostream>


#ifndef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_NO_GLU
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define SHADER_VERSION_STRING "#version 150\n\n"
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_NO_GLU
#include "GLFW/glfw3.h"
#define SHADER_VERSION_STRING "#version 150\n\n"
#endif


#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace Framework
{
	GLuint CreateShader(GLenum eShaderType,
                        const std::string &strShaderFile, const std::string &strShaderName);
	GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
    GLFWwindow* initializeWindow();
    GLuint loadProgram();
}



#endif /* defined(__Shadow_Volumes__Framework__) */
