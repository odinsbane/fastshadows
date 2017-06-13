/*
    Vertex shader with a world view, a light and eventually shadows.
*/

//input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform vec3 cam_offset;
uniform mat4 perspectiveMatrix;
uniform mat4 depthBiasMatrix;

//vec4 camera(vec4 raw_position);

uniform vec3 lightPos;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec4 inputColor;

smooth out vec4 meshColor;
smooth out vec4 shadowCoordinate;



void main() {
    vec4 modelSpace = vec4(position, 1);

    gl_Position = perspectiveMatrix*modelSpace;

    vec4 color = inputColor;
    
    vec3 dirToLight = normalize(lightPos - vec3(position));
    float cosAngIncidence = dot(normal, dirToLight);

    meshColor = (color * lightIntensity*cosAngIncidence ) + color * ambientIntensity;
    
    shadowCoordinate = depthBiasMatrix*modelSpace;
    
}
