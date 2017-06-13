
// Input vertex data, different for all executions of this shader.
//input
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;

void main(){
    
    gl_Position =  depthMVP*vec4(position,1);

}
