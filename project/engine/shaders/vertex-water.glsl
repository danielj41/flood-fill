

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
    aNormal;
    vec4 info = texture2D(uWaterData, aPosition.xz*0.5 + vec2(0.5,0.5));
    vNormal = vec3(normalize(uNormalMatrix*vec4(info.b * 2.0 - 1.0, 1.0, info.a * 2.0 - 1.0, 0.0)));

    gl_Position = uProjection*uView*uModel*vec4(aPosition + vec3(0, -1.0 + info.r * 2.0, 0), 1);
    vWaterColor = texture2D(uWaterColor, aPosition.xz*0.5 + vec2(0.5,0.5));
    vVertex = vec3(gl_Position);
}
