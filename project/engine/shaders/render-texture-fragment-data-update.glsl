uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;

void main(){
    gl_FragColor = vec4((texture2D(uPrevTexture, gl_FragCoord.xy / 256.0).r * 5.0
    +texture2D(uPrevTexture, (gl_FragCoord.xy + vec2(2, 0)) / 256.0).r
    +texture2D(uPrevTexture, (gl_FragCoord.xy - vec2(2, 0)) / 256.0).r
    +texture2D(uPrevTexture, (gl_FragCoord.xy + vec2(0, 2)) / 256.0).r
    +texture2D(uPrevTexture, (gl_FragCoord.xy - vec2(0, 2)) / 256.0).r
    )/9.0, 0.0, 0.0, 1.0);
}
