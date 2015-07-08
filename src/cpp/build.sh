#!/bin/bash
#script for building using pkg-config
#the argmument passed should be the glfw3 package config file.
export GLFW_PC=$1
export GLFW_CFLAGS=$(pkg-config --cflags "$GLFW_PC")
export GLFW_LIBS=$(pkg-config --static --libs "$GLFW_PC")

g++ -std=c++11 $GLFW_CFLAGS *.cpp -lGLEW $GLFW_LIBS -o fastshadows

