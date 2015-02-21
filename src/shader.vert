#version 330 

uniform mat4 modelView;
uniform mat4 modelViewProjection;

in vec3 vert;
in vec3 normal;
in vec4 colour;

out vec3 fVert;
out vec3 fNormal;
out vec4 fColour;

void main() {
    gl_Position = modelViewProjection * vec4(vert, 1.0);

    fVert = vec3(modelView * vec4(vert, 1.0));
    fNormal = normalize(mat3(transpose(inverse(modelView))) * normal); // this may not work once non-uniform scaling is applied
    fColour = colour;
}
