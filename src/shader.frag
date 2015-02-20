#version 330

in vec3 fVert;
in vec3 fNormal;

out vec4 fColor;

void main() {
    // TODO these should be set as uniforms or something
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diffuse = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.4, 0.4, 0.4, 1.0);
    float shininess = 32;

    // for simplicity, the light is just located at the eyepoint
    vec3 L = normalize(vec3(0, 0, 4) - fVert);
    vec3 E = normalize(-fVert);
    vec3 R = normalize(-reflect(L, fNormal));

    // ambient lighting
    vec4 iAmbient = ambient;

    // diffuse lighting
    vec3 surfaceToLight = normalize(L - fVert);

    vec4 iDiff = diffuse * max(dot(fNormal, L), 0.0);
    iDiff = clamp(iDiff, 0.0, 1.0);

    // specular lighting
    vec4 iSpec = specular * pow(max(dot(R, E), 0.0), 0.3 * shininess);
    iSpec = clamp(iSpec, 0.0, 1.0);

    fColor = iAmbient + iDiff + iSpec;
}
