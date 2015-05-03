uniform sampler2D uPrevTexture;
uniform int test;

void main(){
    gl_FragColor = vec4(0.7, texture2D(uPrevTexture, gl_FragCoord.xy / 256.0).g + 0.002*sin(gl_FragCoord.x/256.0*3.14), 0, 1);
}
