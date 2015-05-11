attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

uniform float alpha;
uniform vec3 uVelocity;
uniform vec2 uDTime;

varying vec3 vColor;
varying vec3 vNormal;

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec3 uEmissionColor;
uniform float uShininess;

uniform vec3 uEyePosition;

uniform vec3 uLightPosition;
uniform vec3 uLightDirection;
uniform vec3 uLightColor;

void main(){

    vNormal = aNormal;
    float life = uDTime.y * 10.0 - floor(uDTime.y * 10.0);

    vec4 pos = vec4(aPosition * life * (uDTime.y * 4.0 + 0.5), 1.0);
    pos.x += sin(aTexCoord.x * 5.0 + aTexCoord.y * 6.6 + uDTime.y * 1.5) * 0.5;
    pos.y += cos(aTexCoord.x * 4.0 + aTexCoord.y * 7.6 + uDTime.y * 1.3) * 0.5;
    pos.z = sin(uDTime.y) * 10.0 * aPosition.z * (aTexCoord.x + 0.5) * (aTexCoord.y + 0.5);
    vec4 modelPos = uModel * pos;

    modelPos += vec4(10.0 * uVelocity * uDTime.y, 0.0) - vec4(0.0, 5.0 * uDTime.y * uDTime.y, 0.0, 0.0);

    gl_Position = uProjection * uView * modelPos;

    vec3 kd = uDiffuseColor;
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;
    vec3 Ie = uEmissionColor;
    vec3 Ic = uLightColor;

    float n = uShininess;

    //float d = distance(uLightPosition, vec3(gl_Position));

    vec3 N = normalize(vec3(pos.x, pos.z, pos.y));
    vec3 L = normalize(-uLightDirection);
    vec3 V = normalize(uEyePosition - vec3(gl_Position));

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    vColor = Ic*(Id + Is) + Ia + Ie;
}
