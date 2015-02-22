#version 330

uniform vec4 materialDiffuse;

out vec4 fragColour;

void main() {
    //fragColour = materialDiffuse;
    fragColour = vec4(0.0, 1.0, 0.0, 1.0);
}
