uniform sampler2D uPrevTexture;
uniform int test;

void main(){
    if(test == 10) {
        if(floor(gl_FragCoord.x/20.0) * 20.0 + 10.0 < gl_FragCoord.x) {
            gl_FragColor = vec4(0,0,0,0);
        } else {
            gl_FragColor = vec4(0,1,0,0);
        }
    } else {
        if(floor(gl_FragCoord.x/20.0) * 20.0 + 10.0 < gl_FragCoord.x) {
            gl_FragColor = vec4(0,texture2D(uPrevTexture, gl_FragCoord.xy / 256.0 - vec2(0, 0)).g + 0.01*texture2D(uPrevTexture, gl_FragCoord.xy / 256.0 - vec2(0.02, 0)).g,0,1);
        } else {
            gl_FragColor = vec4(0,texture2D(uPrevTexture, gl_FragCoord.xy / 256.0 - vec2(0, 0)).g - 0.01*texture2D(uPrevTexture, gl_FragCoord.xy / 256.0 - vec2(0.02, 0)).g,0,1);
        }

    }


}
