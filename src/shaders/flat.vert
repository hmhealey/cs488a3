#version 330 

uniform mat4 modelView;
uniform mat4 modelViewProjection;

in vec3 vert;

void main() {
    gl_Position = modelViewProjection * vec4(vert, 1.0);
}
