#version 120
// GUN SHADER

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vTexCoord;

void main(){
    vNormal = vec3(normalize(uNormalMatrix*vec4(aNormal, 0)));
    vVertex = vec3(uModel*vec4(aPosition, 1));
    vTexCoord = aTexCoord;

    gl_Position = uProjection*uView*uModel*vec4(aPosition, 1);
}
