#version 120

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aTangent;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uShadowMatrix;

uniform vec3 uEyePosition;

varying vec2 vTexCoord;
varying vec3 vShadowCoord;
varying vec3 vView;
varying mat3 vTBN;

uniform sampler2D uWaterData;
uniform sampler2D uWaterColor;
uniform sampler2D uWaterBlock;
uniform vec2 uDTime;

varying vec3 vVertex;
varying vec3 vPosition;
varying vec3 vNormal;

mat3 tangentToObjectSpaceMatrix(vec3 tangent, vec3 bitangent, vec3 normal){
    return mat3(tangent, bitangent, normal);
}

void main(){
    float amount = uDTime.y / 1.5;
    amount = amount * amount;

    vec2 coord = aPosition.xz*0.5 + vec2(0.5,0.5);

    vec3 pos = vec3(0, -1.00 + ((1.0 - amount) * texture2D(uWaterData, coord).r * texture2D(uWaterColor, coord).a + amount * texture2D(uWaterBlock, coord).g) * 2.0, 0);

    vec3 pos2 = vec3(0.25, -1.00 + ((1.0 - amount) * texture2D(uWaterData, coord + vec2(1.0/32.0, 0.0)).r * texture2D(uWaterColor, coord + vec2(1.0/32.0, 0.0)).a + amount * texture2D(uWaterBlock, coord + vec2(1.0/32.0, 0.0)).g) * 2.0, 0.0);

    vec3 pos3 = vec3(0.0, -1.00 + ((1.0 - amount) * texture2D(uWaterData, coord + vec2(0.0, 1.0/32.0)).r * texture2D(uWaterColor, coord + vec2(0.0, 1.0/32.0)).a + amount * texture2D(uWaterBlock, coord + vec2(0.0, 1.0/32.0)).g) * 2.0, 0.25);

    vNormal = -cross(pos2 - pos, pos3 - pos);

    gl_Position = uProjection*uView*uModel*vec4(aPosition + pos, 1);
    vVertex = vec3(gl_Position);
    vPosition = aPosition;

    // from vertex-normal-map
    vec3 wPos = vec3(uView*uModel*vec4(aPosition + pos, 1));
    vec3 wNormal  = normalize(vNormal);
    vec3 wTangent = normalize(pos2 - pos);
    vec3 wBitangent = cross(wNormal, wTangent);

    vTexCoord = aTexCoord;
    vView = uEyePosition - wPos;
    vTBN = tangentToObjectSpaceMatrix(wTangent, wBitangent, wNormal);
    vec4 shadowCoord = uShadowMatrix*uModel*vec4(aPosition + pos, 1);
    vShadowCoord = shadowCoord.xyz/shadowCoord.w;
}
