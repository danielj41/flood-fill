attribute vec3 aPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform sampler2D uWaterData;
uniform sampler2D uWaterColor;
uniform vec2 uDTime;

varying vec3 vVertex;
varying vec3 vPosition;
varying vec3 vNormal;

void main(){
    float amount = uDTime.y / 1.5;
    amount = amount * amount;

    vec2 coord = aPosition.xz*0.5 + vec2(0.5,0.5);

    vec3 pos = vec3(0, -1.00 + ((1.0 - amount) * texture2D(uWaterData, coord).r * texture2D(uWaterColor, coord).a + amount * 1.0/uModel[1][1]) * 2.0, 0);

    vec3 pos2 = vec3(1.0, -1.00 + ((1.0 - amount) * texture2D(uWaterData, coord + vec2(1.0/32.0, 0.0)).r * texture2D(uWaterColor, coord + vec2(1.0/32.0, 0.0)).a + amount * 1.0/uModel[1][1]) * 2.0, 0.0);

    vec3 pos3 = vec3(0.0, -1.00 + ((1.0 - amount) * texture2D(uWaterData, coord + vec2(0.0, 1.0/32.0)).r * texture2D(uWaterColor, coord + vec2(0.0, 1.0/32.0)).a + amount * 1.0/uModel[1][1]) * 2.0, 1.0);

    vNormal = -cross(pos2 - pos, pos3 - pos);

    gl_Position = uProjection*uView*uModel*vec4(aPosition + pos, 1);
    vVertex = vec3(gl_Position);
    vPosition = aPosition;
}
