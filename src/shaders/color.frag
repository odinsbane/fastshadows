/*
    Fragment shader for lit, shadow casting project.
*/

//version defined in program

//position ordinates are located in gl_FragCoord
//and are in 'window coordinates'
out vec4 outputColor;

uniform sampler2D shadowMap;
smooth in vec4 shadowCoordinate;

smooth in vec4 meshColor;
float add(float a, float b);

float linearize(float depth)
{
    if(depth<0) depth = -depth;
    const float near = 0.1, far = 10.0;
    return 2.0 * near / (far + near - depth * (far - near));
}

void main() {
    float bias = 0.01;
    if( texture( shadowMap, shadowCoordinate.xy ).x  <  shadowCoordinate.z-bias){
        outputColor = meshColor - vec4(0.9, 0.9, 0.9, 0);
    } else{
        outputColor = meshColor;
    }
}


float add(float a, float b){
    
    float c = (a + b);
    if(c<0) c = -c;
    
    if(c>1) c =1.0/c;
    return c;
    
}
