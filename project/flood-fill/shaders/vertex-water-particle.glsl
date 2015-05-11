attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform float alpha;
uniform vec3 uVelocity;
uniform vec2 uDTime;

varying vec3 vColor;
varying vec3 vNormal;
varying float vLife;

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
    float distortedTime =  0.3 * (uDTime.y + aTexCoord.x) * (aTexCoord.x * 3.14 + 1.14 * aTexCoord.y * 1.44 + 0.57);
    vLife = (distortedTime - floor(distortedTime)) * 0.5;

    vec3 rotPos = aPosition;
    rotPos.z = aPosition.x * vLife * uDTime.y + 1.5 * aPosition.y * vLife * uDTime.y;

    float lifeTime = (uDTime.y + 0.02) * (vLife * 1.5 + 0.1);

    vec4 pos = vec4(rotPos * vLife * vLife * 1.0, 1.0);
    pos.x += sin(aTexCoord.x * 5.0 + aTexCoord.y * 6.6 + uDTime.y * 0.5) * vLife * vLife * vLife * 2.0;
    pos.y += cos(aTexCoord.x * 4.0 + aTexCoord.y * 7.6 + uDTime.y * 0.6) * vLife * vLife * vLife * 2.0;
    pos.z = sin(uDTime.y) * 10.0 * aPosition.z * (aTexCoord.x + 0.5) * (aTexCoord.y + 0.5);
    vec4 modelPos = uModel * pos;

    float multiplier = lifeTime + 0.3;

    modelPos += vec4(10.0 * uVelocity * multiplier * lifeTime, 0.0) - vec4(0.0, 5.0 * multiplier * multiplier * lifeTime * lifeTime, 0.0, 0.0);

    gl_Position = uProjection * uView * modelPos;

    vec3 kd = uDiffuseColor;
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;
    vec3 Ie = uEmissionColor;
    vec3 Ic = uLightColor;

    float n = uShininess;

    //float d = distance(uLightPosition, vec3(gl_Position));

    vec3 N = normalize(vNormal);
    vec3 L = normalize(-uLightDirection);
    vec3 V = normalize(uEyePosition - vec3(gl_Position));

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    vColor = Ic*(Id + Is) + Ia + Ie;
}
