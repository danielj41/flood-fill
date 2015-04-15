#version 120

attribute vec4 aPosition;
attribute vec4 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

varying vec4 vColor;

void main(){
    gl_Position = uProjection*uView*uModel*aPosition;
    vColor = (uModel*aNormal)*0.5 + 1;
}
