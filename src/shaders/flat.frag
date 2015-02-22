#version 330

uniform vec4 materialDiffuse;

out vec4 fragColour;

void main() {
    fragColour = materialDiffuse;
}
