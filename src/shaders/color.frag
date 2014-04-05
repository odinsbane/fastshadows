/*
    Fragment shader for lit, shadow casting project.
*/

//version defined in program

//position ordinates are located in gl_FragCoord
//and are in 'window coordinates'
out vec4 outputColor;


smooth in vec4 meshColor;
void main() {
    outputColor = meshColor;
}


