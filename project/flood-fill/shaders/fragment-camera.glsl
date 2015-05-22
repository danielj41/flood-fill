#version 120
// GUN SHADER

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform float uShininess;

uniform vec3 uEyePosition;
uniform vec3 uLightDirection;

uniform sampler2D uTexID;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vTexCoord;

void main(){
    vec4 texel = texture2D(uTexID, vTexCoord);

    vec3 colorkey = vec3(1, 0, 1);
    vec3 ks = uSpecularColor;
    vec3 Ia = uAmbientColor;
    vec3 kd;

    if(texel.xyz == colorkey){
        kd = uDiffuseColor;
    }
    else{
        kd = texel.xyz;
    }

    float n = uShininess;

    vec3 N = normalize(vec3(vNormal));
    vec3 L = normalize(-uLightDirection);
    vec3 V = normalize(uEyePosition - vVertex);

    vec3 H = normalize(L + V);

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    vec3 I;

    if(texel.xyz == colorkey){
        I = (Id + Is) + Ia;
        gl_FragColor = vec4(I, 1.0);
    }
    else{
        I = (Id + Is) + texel.xyz;
        gl_FragColor = vec4(I, 1.0)*texel;
    }
}
