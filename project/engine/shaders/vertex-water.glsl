

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

uniform sampler2D uWaterData;
uniform sampler2D uWaterColor;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec4 vWaterColor;

void main(){

    vNormal = vec3(normalize(uNormalMatrix*vec4(aNormal, 0)));
    vVertex = vec3(uProjection*uView*uModel*vec4(aPosition, 1));

    gl_Position = uProjection*uView*uModel*vec4(aPosition - vec3(0, texture2D(uWaterData, aPosition.xz*0.5 + 0.5).g, 0), 1);
    vWaterColor = texture2D(uWaterColor, aPosition.xz*0.5 + 0.5);
}
