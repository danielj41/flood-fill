varying float vHeight;
varying float vZ;

void main(){
    gl_FragColor = vec4((vHeight + 1.0) / 2.0, (vHeight + 1.0) / 2.0, (vHeight + 1.0) / 2.0, 1);
}
