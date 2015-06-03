#version 120

varying vec2 vTexCoord;
varying vec3 vView;
varying mat3 vTBN;
varying vec3 vShadowCoord;

uniform sampler2D uTextureID;
uniform sampler2D uNormalTexID;
uniform sampler2D uShadowTexID;

uniform vec2 uGridScale;
// Paralax Mapping
//uniform float uNormalMapBias;
//uniform float uNormalMapScale;

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec2 uScreenSize;
uniform float uShininess;

uniform vec3 uLightDirection;

vec2 getTexCoordOffset(vec2 texCoord){
    texCoord.x = (texCoord.x * uGridScale.x);
    texCoord.y = (texCoord.y * uGridScale.y);
    return texCoord;
}

float getShadow(vec3 shadowCoord){
    // The Shadow UV coordinate must be between [0, 1]
    // You can use linear interpolation to get it: 0.5*value + 0.5

    // You need to apply the homogeneous divide in the UV vector

    float shadowmap =  texture2D(uShadowTexID, shadowCoord.xy).x;

    if(shadowmap + 0.0001 < shadowCoord.z){
        return 0.2f;
    }
    return 1.0f;
}

float pcfShadow(){
    vec3 shadowUV = vShadowCoord;
    shadowUV = 0.5f*shadowUV + 0.5f;

    float xOffset = 1.0f/uScreenSize.x;
    float yOffset = 1.0f/uScreenSize.x;

    float shadowness = 0.0f;

    // Must be an even number
    int kernel_size = 3;
    for(int y = -kernel_size/2; y <= kernel_size/2; y++){
        for(int x = -kernel_size/2; x <= kernel_size/2; x ++){
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            shadowness += getShadow(vec3(shadowUV.xy + Offsets, shadowUV.z));
        }
    }

    shadowness = shadowness/(kernel_size*kernel_size);
    return shadowness;
}

/*vec2 getTexCoordOffset(vec2 texCoord){
    // Parallax Mapping
    float parallaxScale = uNormalMapScale;
    float bias = uNormalMapBias;
    vec3 tanView = normalize(transpose(vTBN)*vView);

    for(int i = 0; i < 4; i++){
        vec4 uvNormal = 2*texture2D(uNormalTexID, texCoord) - 1;
        float h = uvNormal.w*parallaxScale + bias;
        texCoord += tanView.xy * uvNormal.z * h;
    }

    return texCoord;
}*/

void main(){
    vec2 newTexCoord = getTexCoordOffset(vTexCoord);

    vec3 texNormal = 2*vec3(texture2D(uNormalTexID, newTexCoord)) - 1;
    vec4 texel = texture2D(uTextureID, newTexCoord);

    vec3 normal = vTBN*texNormal;

    vec3 kd = uDiffuseColor;
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;

    float n = uShininess;

    vec3 N = normalize(normal);
    vec3 L = normalize(uLightDirection);
    vec3 V = normalize(vView);

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    vec3 I = pcfShadow()*(Id + Is) + Ia;

    gl_FragColor = vec4(vec3(I), 1)*texel;
}
