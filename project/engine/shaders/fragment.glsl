#version 120

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec3 uEmissionColor;
uniform float uShininess;

uniform vec3 uEyePosition;

//uniform vec3 uLightPosition;
uniform vec3 uLightDirection;
uniform vec3 uLightColor;

varying vec3 vVertex;
varying vec3 vNormal;

void main(){
    vec3 kd = uDiffuseColor;
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;
    vec3 Ie = uEmissionColor;
    vec3 Ic = uLightColor;

    float n = uShininess;

    //float d = distance(uLightPosition, vVertex);

    vec3 N = normalize(vec3(vNormal));
    vec3 L = normalize(-uLightDirection);
    vec3 V = normalize(uEyePosition - vVertex);

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0), n)*ks;
    vec3 Id = max(dot(N, L), 0)*kd;

    //float attenuation = dot(uLightFallOff, vec3(1, d, d*d));

    vec3 I = Id + Is + Ia + Ie;

    gl_FragColor = vec4(I, 1);
}
