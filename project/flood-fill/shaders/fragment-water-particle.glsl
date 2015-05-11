varying vec3 vColor;
varying vec3 vNormal;
varying float vLife;

uniform vec2 uDTime;

void main(){
    float opacity = clamp(0.1 / (uDTime.y - 0.5) / (uDTime.y - 0.5), 0.0, 1.0) * 0.5 * (1.0 - vLife) * (1.0 - vLife) * pow(clamp(1.0 - (dot(vNormal, vNormal) - 0.3267)/0.1733, 0.0, 1.0), 0.25);
    gl_FragColor = vec4((1.0 - vColor.r) * opacity + vColor.r, (1.0 - vColor.g) * opacity + vColor.g, (1.0 - vColor.b) * opacity + vColor.b, opacity * 2.0);
}
