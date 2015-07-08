//
//  Framework.cpp
//  Shadow Volumes
//
//  Created by msmith on 4/4/14.
//  Copyright (c) 2014 paluchlab. All rights reserved.
//

#include "Framework.h"



namespace Framework
{
	GLuint CreateShader(GLenum eShaderType,
                        const std::string &strShaderFile, const std::string &strShaderName)
	{
		GLuint shader = glCreateShader(eShaderType);
		const char *strFileData = strShaderFile.c_str();
		glShaderSource(shader, 1, &strFileData, NULL);
        
		glCompileShader(shader);
        
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            
			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
            
			const char *strShaderType = NULL;
			switch(eShaderType)
			{
			    case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			    case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			    case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
			}
            
			fprintf(stderr, "Compile failure in %s shader named \"%s\". Error:\n%s\n",
                    strShaderType, strShaderName.c_str(), strInfoLog);
			delete[] strInfoLog;
		}
        
		return shader;
	}
    
	GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
	{
		std::string strFilename = strShaderFilename;
		std::ifstream shaderFile(strFilename.c_str());
		std::stringstream shaderData;
        shaderData << SHADER_VERSION_STRING;
		shaderData << shaderFile.rdbuf();
		shaderFile.close();
		
        
		return CreateShader(eShaderType, shaderData.str(), strShaderFilename);
	}
	
	GLuint CreateProgram(const std::vector<GLuint> &shaderList)
    {
        GLuint program = glCreateProgram();
        
        for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
            glAttachShader(program, shaderList[iLoop]);
        
        glLinkProgram(program);
        
        GLint status;
        glGetProgramiv (program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
            
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
            fprintf(stderr, "Linker failure: %s\n", strInfoLog);
            delete[] strInfoLog;
        }
        
        return program;
    }
    
    GLFWwindow* initializeWindow(){
        
        /*
         *Creating and initlizing a window on mac.
         *
         **/
        GLFWwindow* window;
        
        if( glfwInit() != GL_TRUE ) {
            exit( EXIT_FAILURE );
        }
        
        
        /* Initialize the library */
        //if (!glfwInit())
        //    return 0;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        

        window = glfwCreateWindow(640, 480, "Shadow Volumes", NULL, NULL);
        glfwMakeContextCurrent(window);
        
#ifndef __APPLE__
        glewExperimental=true;
        glewInit();
#endif
        /* Create a windowed mode window and its OpenGL context */
        
        if (!window)
        {
            glfwTerminate();
            return 0;
        }
        
        /* Make the window's context current */
        
        printf("GLSL version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("GL Version: %s\n", glGetString(GL_VERSION));
        
        //exit(0);
        
        glEnable(GL_DEPTH_TEST);
        
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glDepthRange(0.0f, 1.0f);
        
        glClearColor( 0.02f, 0.02f, 0.02f, 0.0f );
        glClearDepth( 1.0f );
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        printf("setup\n");
        
        return window;
        
        
    }
    
    GLuint loadProgram(){
        //Load program.
        std::vector<GLuint> shaderList;
        
        shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "shaders/world.vert"));
        shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "shaders/color.frag"));
        return Framework::CreateProgram(shaderList);
    }
    
    
}
