/*
    Vertex shader with a world view, a light and eventually shadows.
*/

//input
in vec3 position;
in vec3 normal;

uniform vec3 cam_offset;
uniform mat4 perspectiveMatrix;
uniform mat4 orientationMatrix;
uniform mat3 normalModelToCameraMatrix;

//vec4 camera(vec4 raw_position);

uniform vec3 lightPos;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec4 inputColor;

smooth out vec4 meshColor;

void main() {
    vec4 camPosition = orientationMatrix*(vec4(position, 1) + vec4(cam_offset,0));
    
    gl_Position = perspectiveMatrix*camPosition;
    
    vec4 color = inputColor;
    //vec3 nn = normalize(normal);
    vec3 dirToLight = normalize(lightPos - vec3(position));
    float cosAngIncidence = dot(normal, dirToLight);

    meshColor = (color* lightIntensity*cosAngIncidence ) + color * ambientIntensity;
}
