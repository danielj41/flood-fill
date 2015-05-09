

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
uniform vec2 uDTime;

varying vec3 vVertex;
varying vec3 vPosition;
varying vec3 vNormal;

void main(){
    vec2 coord = vPosition.xz*0.5 + vec2(0.5,0.5);
    vec4 info = texture2D(uWaterData, coord);
    vec4 color = texture2D(uWaterColor, coord);
    vec4 block = texture2D(uWaterBlock, coord);
    vec4 blockCenter = texture2D(uWaterBlock, vec2((floor(coord.x*uModel[0][0]) + 0.5)/uModel[0][0], (floor(coord.y*uModel[2][2]) + 0.5)/uModel[2][2]));

    uDiffuseColor;
    vec3 kd = vec3(uDiffuseColor.r * color.r, uDiffuseColor.g * color.g, uDiffuseColor.b * color.b);
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

    float amount = uDTime.y / 1.5;
    amount = amount * amount;
    float blockHeight = (1.0 - amount) * block.r + amount * blockCenter.r;

    gl_FragColor = vec4(I, color.a * clamp(uModel[1][1] * (max(info.r, blockHeight) - blockHeight), 0.0, 1.0));
}
