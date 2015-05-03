varying float vHeight;

void main(){
    gl_FragColor = vec4((vHeight + 1.0) / 2.0, (vHeight + 1.0) / 2.0, (vHeight + 1.0) / 2.0, 1);
}
