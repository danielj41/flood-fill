

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform vec3 uAmbientColor;
uniform vec3 uEmissionColor;
uniform float uShininess;

uniform vec3 uEyePosition;

//uniform vec3 uLightPosition;
uniform vec3 uLightDirection;
uniform vec3 uLightColor;

uniform mat4 uModel;
uniform mat4 uNormalMatrix;
uniform sampler2D uWaterData;
uniform sampler2D uWaterColor;
uniform sampler2D uWaterBlock;

varying vec3 vVertex;
varying vec3 vPosition;
varying vec3 vNormal;

void main(){
    vec4 info = texture2D(uWaterData, vPosition.xz*0.5 + vec2(0.5,0.5));
    vec4 color = texture2D(uWaterColor, vPosition.xz*0.5 + vec2(0.5,0.5));
    vec4 block = texture2D(uWaterBlock, vPosition.xz*0.5 + vec2(0.5,0.5));

    uDiffuseColor;
    vec3 kd = color.rgb;
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

    vec3 Is = pow(max(dot(N, H), 0.0), n)*ks;
    vec3 Id = max(dot(N, L), 0.0)*kd;

    //float attenuation = dot(uLightFallOff, vec3(1, d, d*d));

    vec3 I = Ic*(Id + Is) + Ia + Ie;

    gl_FragColor = vec4(I, color.a * min(1.0, uModel[3][1] * ((info.r * color.a) - block.r)));
}
