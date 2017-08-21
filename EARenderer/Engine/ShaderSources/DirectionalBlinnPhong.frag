#version 400 core

// Constants

const int kMaxCascades = 4;

// Output

out vec4 outputFragColor;

// Input

in vec3 oNormal;
in vec3 oToCamera;
in vec3 oTexCoords;
in vec4 oPosInLightSpace[kMaxCascades];
in vec4 oPosInCameraSpace;

// Uniforms

struct Material {
    sampler2D diffuseTexture;
    vec3 ambientReflectances;
    vec3 diffuseReflectances;
    vec3 specularReflectances;
    float specularExponent;
};

uniform Material uMaterial;
uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform sampler2DArray uShadowMaps;
uniform float uDepthSplits[kMaxCascades];
uniform vec4 uHighlightColor;

uniform int uNumberOfCascades;

// Functions

vec3 ambientColor() {
    return uMaterial.ambientReflectances * uLightColor;
}

vec3 diffuseColor(in vec3 N, in vec3 L) {
    float cosTheta = max(dot(N, L), 0.0);
    return uMaterial.diffuseReflectances * uLightColor * cosTheta;
}

vec3 specularColor(in vec3 N, in vec3 L, in vec3 V) {
    vec3 H = normalize(L + V);
    float specularContribution = pow(max(dot(N, H), 0.0), uMaterial.specularExponent);
    return uMaterial.specularReflectances * uLightColor * specularContribution;
}

int cascadeIndex()
{
    vec3 projCoords = oPosInCameraSpace.xyz / oPosInCameraSpace.w;
    // No need to transform to [0,1] range,
    // because splits passed from client are in [-1; 1]
    
    float fragDepth = projCoords.z;

    for (int i = 0; i < uNumberOfCascades; ++i) {
        if (fragDepth < uDepthSplits[i]) {
            return i;
        }
    }
}

float isInShadow(in vec3 N, in vec3 L)
{
    int shadowCascadeIndex = cascadeIndex();
    
    // perform perspective division
    vec3 projCoords = oPosInLightSpace[shadowCascadeIndex].xyz / oPosInLightSpace[shadowCascadeIndex].w;

    // Transformation to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowMaps, vec3(projCoords.xy, shadowCascadeIndex)).r;

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(N, L)), 0.005);

    float shadow = 0.0;
    vec3 texelSize = 1.0 / textureSize(uShadowMaps, 0);
    for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            vec2 xy = projCoords.xy + vec2(x, y) * texelSize.xy;
            float pcfDepth = texture(uShadowMaps, vec3(xy, shadowCascadeIndex)).r;
            float unbiasedDepth = currentDepth - bias;
            shadow += unbiasedDepth > pcfDepth && unbiasedDepth <= 1.0 ? 1.0 : 0.0;
        }
    }
    shadow /= 36.0;
    return shadow;
//    float unbiasedDepth = currentDepth - bias;
//    return unbiasedDepth > closestDepth && // Check of being in shadow
//    unbiasedDepth <= 1.0 ? 1.0 : 0.0; // Depth values > 1.0 are considered to not being in shadow
}

void main() {
    // Invert light direction because we need TO light vector, not FROM light vector to perform Half vector computations
    vec3 L = normalize(-uLightDirection);
    vec3 V = normalize(oToCamera);
    vec3 N = normalize(oNormal);

    vec3 ambientColor = ambientColor();
    vec3 diffuseColor = diffuseColor(N, L);
    vec3 specularColor = specularColor(N, L, V);
    
    outputFragColor = vec4((ambientColor + (1.0 - isInShadow(N, L)) * (diffuseColor + specularColor)), 1.0) * texture(uMaterial.diffuseTexture, oTexCoords.st) + uHighlightColor;

    int shadowCascadeIndex = cascadeIndex();
    if (shadowCascadeIndex == 0) {
        outputFragColor += vec4(0.3, 0.0, 0.0, 0.0);
    } else if (shadowCascadeIndex == 1) {
        outputFragColor += vec4(0.0, 0.3, 0.0, 0.0);
    } else if (shadowCascadeIndex == 2) {
        outputFragColor += vec4(0.0, 0.0, 0.3, 0.0);
    }
}
