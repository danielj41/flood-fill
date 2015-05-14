#version 130

varying vec2 vTexCoord;
varying vec3 vView;
varying mat3 vTBN;
varying vec4 vShadowCoord;

uniform sampler2D uTextureID;
uniform sampler2D uNormalTexID;
uniform sampler2D uShadowTexID;

// Paralax Mapping
//uniform float uNormalMapBias;
//uniform float uNormalMapScale;

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec2 uScreenSize;
uniform float uShininess;

uniform vec3 uLightDirection;

float getShadow(){
    vec3 shadowUV = vShadowCoord.xyz/vShadowCoord.w;
    shadowUV = 0.5f*shadowUV + 0.5f;
    //float shadowmap =  texture2D(uShadowTexID, shadowUV.xy).x;

    float xOffset = 1.0f/uScreenSize.x;
    float yOffset = 1.0f/uScreenSize.x;

    float shadowness = 0.0f;

    // Must be an even number
    int kernel_size = 5;
    for(int y = -kernel_size/2; y <= kernel_size/2; y++){
        for(int x = -kernel_size/2; x <= kernel_size/2; x ++){
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            shadowness += texture2D(uShadowTexID, shadowUV.xy + Offsets).x;
        }
    }

    shadowness = shadowness/(kernel_size*kernel_size);
    if(shadowness + 0.001 < shadowUV.z){
        return 0.2f;
    }
    return 1.0f;

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
    vec3 texNormal = 2*vec3(texture2D(uNormalTexID, vTexCoord)) - 1;
    vec4 texel = texture2D(uTextureID, vTexCoord);

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

    vec3 I = getShadow()*(Id + Is) + Ia;

    gl_FragColor = vec4(vec3(I), 1)*texel;
}
