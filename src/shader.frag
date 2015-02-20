#version 330

in vec3 fVert;
in vec3 fNormal;

out vec4 colour;

void main() {
    vec4 base = vec4(1.0, 0.0, 0.0, 1.0);

    vec3 light = vec3(0, 0, -4);
    vec3 surfaceToLight = normalize(light - fVert);

    vec4 diff = base * max(dot(fNormal, surfaceToLight), 0);

    colour = diff;
    //colour = vec4(1.0, 0.0, 0.0, 1.0);
}
