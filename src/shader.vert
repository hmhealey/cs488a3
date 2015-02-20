#version 330 

uniform mat4 mvpMatrix;

in vec3 vert;
in vec3 normal;

out vec3 fVert;
out vec3 fNormal;

void main() {	
    //gl_Position = mvpMatrix * vec4(vert, 1.0);
    gl_Position = vec4(vert, 1.0);
    // gl_Position = ftransform();

    fVert = vert;
    fNormal = normal;
}
