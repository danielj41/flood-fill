

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

uniform sampler2D uWaterData;
uniform sampler2D uWaterColor;
uniform sampler2D uWaterBlock;
uniform vec2 uDTime;

varying vec3 vVertex;
varying vec3 vPosition;
varying vec3 vNormal;

void main(){
    aNormal;
    uNormalMatrix;
    uWaterBlock;
    vec2 coord = aPosition.xz*0.5 + vec2(0.5,0.5);
    vec4 info = texture2D(uWaterData, coord);
    vec4 color = texture2D(uWaterColor, coord);
    vNormal = vec3(uNormalMatrix * vec4(normalize(vec3(info.b * 2.0 - 1.0, 2.0, info.a * 2.0 - 1.0)), 0.0));

    float amount = uDTime.y / 1.5;
    amount = amount * amount;

    gl_Position = uProjection*uView*uModel*vec4(aPosition + vec3(0, -1.00 + ((1.0 - amount) * info.r * color.a + amount * 1.0/uModel[1][1]) * 2.0, 0), 1);
    vVertex = vec3(gl_Position);
    vPosition = aPosition;
}
