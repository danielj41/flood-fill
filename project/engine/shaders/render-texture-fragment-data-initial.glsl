uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;

void main(){
    gl_FragColor = vec4(texture2D(uDataTexture, gl_FragCoord.xy / 512.0).r + 0.1, 0.5, 0.0, 1.0);
}
