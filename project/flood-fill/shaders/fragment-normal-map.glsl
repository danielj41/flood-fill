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
    float shadowmap =  texture2D(uShadowTexID, vec2(vShadowCoord.xy/vShadowCoord.w)*0.5f + 0.5f).x;
    if(shadowmap + 0.00001 < 0.5*vShadowCoord.z/vShadowCoord.w + 0.5){
        return 0.0f;
    }
    return shadowmap;
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

    vec3 kd = vec3(texel);
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

    vec3 I = getShadow()*(Id + Is) + Ia*Id;

    gl_FragColor = vec4(vec3(I), 1);
}
