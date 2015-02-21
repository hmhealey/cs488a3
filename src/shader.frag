#version 330

uniform vec4 materialAmbient;
uniform vec4 materialSpecular;
uniform vec4 materialDiffuse;
uniform float materialShininess;
uniform vec4 colour;

in vec3 fVert;
in vec3 fNormal;

out vec4 fragColour;

void main() {
    // TODO set the light components as uniforms r something
    vec4 ambient = materialAmbient * vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diffuse = materialDiffuse * vec4(0.8, 0.8, 0.8, 1.0);
    vec4 specular = materialSpecular * vec4(0.4, 0.4, 0.4, 1.0);
    float shininess = materialShininess;

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

    fragColour = iAmbient + colour * iDiff + iSpec;
}
