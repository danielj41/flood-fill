#version 130

varying vec2 vTexCoord;
varying vec3 vView;
varying mat3 vTBN;
varying vec4 vShadowCoord;

uniform sampler2D uTextureID;
uniform sampler2D uNormalTexID;
uniform sampler2D uShadowTexID;

uniform float uNormalMapBias;
uniform float uNormalMapScale;

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec3 uEmissionColor;
uniform float uShininess;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;

float getShadow(){
    vec3 shadowUV = vShadowCoord.xyz/vShadowCoord.w;
    shadowUV = 0.5f*shadowUV + 0.5f;
    //float shadowmap =  texture2D(uShadowTexID, shadowUV.xy).x;

    //TODO: get screen size
    float xOffset = 1.0f/600.0f;
    float yOffset = 1.0f/600.0f;

    float shadowness = 0.0f;

    int kernel_size = 5;
    for(int y = -kernel_size/2; y <= kernel_size/2; y++){
        for(int x = -kernel_size/2; x <= kernel_size/2; x ++){
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            shadowness += texture2D(uShadowTexID, shadowUV.xy + Offsets).x;
        }
    }

    shadowness = shadowness/(kernel_size*kernel_size);
    //shadowness = texture2D(uShadowTexID, shadowUV.xy).x;
    if(shadowness + 0.001 < shadowUV.z){
        return shadowness;
    }
    return 1.0f;

}

vec2 getTexCoordOffset(vec2 texCoord){
    float parallaxScale = uNormalMapScale;
    float bias = uNormalMapBias;
    vec3 tanView = normalize(transpose(vTBN)*vView);

    /*for(int i = 0; i < 4; i++){
        vec4 uvNormal = 2*texture2D(uNormalTexID, texCoord) - 1;
        float h = uvNormal.w*parallaxScale + bias;
        texCoord += tanView.xy * uvNormal.z * h;
    }*/
    vec4 uvNormal = 2*texture2D(uNormalTexID, texCoord) - 1;
    float h = uvNormal.w*parallaxScale + bias;
    texCoord += tanView.xy * uvNormal.z * h;

    return texCoord;
}

void main(){
    vec2 offsetTexCoord = getTexCoordOffset(vTexCoord);

    vec3 texNormal = 2*vec3(texture2D(uNormalTexID, offsetTexCoord)) - 1;
    vec4 texel = texture2D(uTextureID, offsetTexCoord);

    vec3 normal = vTBN*texNormal;

    vec3 kd = uDiffuseColor;
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;
    vec3 Ie = uEmissionColor;
    vec3 Ic = uLightColor;

    float n = uShininess;

    vec3 N = normalize(normal);
    vec3 L = normalize(-uLightDirection);
    vec3 V = normalize(vView);

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    vec3 I = getShadow()*(Id + Is) + Ia;

    gl_FragColor = vec4(vec3(I), 1)*texel;
}
