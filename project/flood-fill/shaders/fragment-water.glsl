#version 120

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec3 uEmissionColor;
uniform float uShininess;

uniform vec3 uEyePosition;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform mat4 uModel;
uniform sampler2D uWaterData;
uniform sampler2D uWaterColor;
uniform sampler2D uWaterBlock;
uniform vec2 uDTime;

uniform sampler2D uTextureID;
uniform sampler2D uNormalTexID;
uniform sampler2D uShadowTexID;

uniform vec2 uGridScale;
uniform vec2 uScreenSize;

varying vec3 vVertex;
varying vec3 vPosition;
varying vec3 vNormal;

varying vec2 vTexCoord;
varying vec3 vView;
varying mat3 vTBN;
varying vec3 vShadowCoord;


vec2 getTexCoordOffset(vec2 texCoord){
    texCoord.x = (texCoord.x * uGridScale.x) - floor(texCoord.x * uGridScale.x);
    texCoord.y = (texCoord.y * uGridScale.y) - floor(texCoord.y * uGridScale.y);
    return texCoord;
}

float getShadow(vec3 shadowCoord){
    // The Shadow UV coordinate must be between [0, 1]
    // You can use linear interpolation to get it: 0.5*value + 0.5

    // You need to apply the homogeneous divide in the UV vector

    float shadowmap =  texture2D(uShadowTexID, shadowCoord.xy).x;

    if(shadowmap + 0.00001 < shadowCoord.z){
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

void main(){
    vec2 coord = vPosition.xz*0.5 + vec2(0.5,0.5);
    vec4 info = texture2D(uWaterData, coord);
    vec4 color = texture2D(uWaterColor, coord);
    vec4 block = texture2D(uWaterBlock, coord);
    vec4 blockCenter = texture2D(uWaterBlock, vec2((floor(coord.x*uModel[0][0]) + 0.5)/uModel[0][0], (floor(coord.y*uModel[2][2]) + 0.5)/uModel[2][2]));

    vec2 newTexCoord = getTexCoordOffset(vTexCoord);

    vec3 texNormal = 2*vec3(texture2D(uNormalTexID, newTexCoord)) - 1;
    vec4 texel = texture2D(uTextureID, newTexCoord);

    vec3 normal = vTBN*texNormal;

    vec3 kd = vec3(uDiffuseColor.r * color.r, uDiffuseColor.g * color.g, uDiffuseColor.b * color.b);
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;
    vec3 Ie = uEmissionColor;
    vec3 Ic = uLightColor;

    float n = uShininess;

    vec3 N = normalize(normal);
    vec3 L = normalize(uLightDirection);
    vec3 V = normalize(vView);

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    vec3 I = pcfShadow()*(Id + Is) + Ia;

    float amountLin = uDTime.y / 1.5;
    float amount = amountLin * amountLin;
    float blockHeight = (1.0 - amountLin) * block.r + amountLin * blockCenter.r;
    float diff = 2.75 * uModel[1][1] * (max(((1.0 - amount) * info.r * color.a + amount * block.g), blockHeight) - blockHeight - 0.05/uModel[1][1]);

    gl_FragColor = vec4(I, color.a * clamp(pow(max(diff, 0.0), 0.35), 0.0, 1.0));
}
