varying vec3 vColor;
varying vec3 vNormal;

uniform vec2 uDTime;
uniform float alpha;

void main(){

    float life = uDTime.y * 10.0 - floor(uDTime.y * 10.0);

    gl_FragColor = vec4(vColor, (1.0 - life) * (2.5 - 6.0 * dot(vNormal, vNormal)));
}
