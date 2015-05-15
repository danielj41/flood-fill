#version 120

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
attribute vec4 aTangent;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;
uniform mat4 uShadowMatrix;

uniform vec3 uEyePosition;

varying vec2 vTexCoord;
varying vec3 vShadowCoord;
varying vec3 vView;
varying mat3 vTBN;

/*mat3 objectToTangentSpaceMatrix(vec3 tangent, vec3 bitangent, vec3 normal){
    // Tangent space is the space where the Texture is. :: TBN^-1
    //   | T'x T'y T'z |
    //   | B'x B'y B'z |
    //   | N'x N'y N'z |

    return transpose(mat3(tangent, bitangent, normal));
}*/

mat3 tangentToObjectSpaceMatrix(vec3 tangent, vec3 bitangent, vec3 normal){
    return mat3(tangent, bitangent, normal);
}

void main(){
    //World Space
    vec3 wPos = vec3(uView*uModel*vec4(aPosition, 1));
    vec3 wNormal  = normalize(vec3(uNormalMatrix*vec4(aNormal, 0.0f)));
    vec3 wTangent = normalize(vec3(uNormalMatrix*vec4(aTangent.xyz, 0.0f)));
    vec3 wBitangent = cross(wNormal, wTangent) * aTangent.w;

    //Position Output
    vTexCoord = aTexCoord;
    vView = uEyePosition - wPos;
    vTBN = tangentToObjectSpaceMatrix(wTangent, wBitangent, wNormal);
    vec4 shadowCoord = uShadowMatrix*uModel*vec4(aPosition, 1);
    vShadowCoord = shadowCoord.xyz/shadowCoord.w;

    gl_Position = uProjection*uView*uModel*vec4(aPosition, 1);
}
