varying float vHeight;
varying float vZ;
uniform vec3 uColor;
uniform sampler2D uPrevTexture;

void main(){
    float value = (vHeight + 1.0) / 2.0;
    gl_FragColor = texture2D(uPrevTexture, gl_FragCoord.xy/256.0) + vec4(uColor * value, 1.0);
}
