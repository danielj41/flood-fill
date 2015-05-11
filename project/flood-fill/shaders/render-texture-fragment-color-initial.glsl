uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;

void main(){
    if(texture2D(uDataTexture, gl_FragCoord.xy / 256.0).r < 0.99) {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
    }
}
