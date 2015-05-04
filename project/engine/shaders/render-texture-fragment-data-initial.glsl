uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;

void main(){
    gl_FragColor = vec4(min(texture2D(uDataTexture, gl_FragCoord.xy / 512.0).r - 0.5/uSize.y, 1.0 - 1.0/uSize.y), 0.5, 0.0, 1.0);
}
