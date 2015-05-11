

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;
uniform vec3 uVelocity;
uniform vec2 uDTime;

varying vec3 vVertex;
varying vec3 vNormal;
uniform float alpha;
varying float vMultiplier;

void main(){
    alpha;
    vNormal = vec3(normalize(uNormalMatrix*vec4(aNormal, 0)));
    vVertex = vec3(uProjection*uView*uModel*vec4(aPosition, 1));

    float multiplier = (3.0 + aPosition.z) / 4.0 + 0.25;
    vec4 modelPos = vec4((sin(multiplier * aPosition.z*5.0 + uDTime.y*4.0)*sin(multiplier * aPosition.z*5.0 + uDTime.y*4.0)+0.5)*aPosition * (12.0 * uDTime.y * multiplier * uDTime.y * multiplier + 1.0), 1);
    modelPos.z = aPosition.z * 2.0;
    modelPos = uModel * modelPos;

    modelPos += vec4(10.0 * uVelocity * uDTime.y, 0.0) + vec4(0.0, 6.0 * sign(uVelocity.y) * uVelocity.y * uVelocity.y * aPosition.z, 0.0, 0.0) - vec4(0.0, 5.0 * uDTime.y * uDTime.y * multiplier * multiplier, 0.0, 0.0);

    gl_Position = uProjection * uView * modelPos;
    vMultiplier = multiplier;
}
