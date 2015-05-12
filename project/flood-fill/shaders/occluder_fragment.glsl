#version 120

void main(){
    gl_FragDepth = gl_FragCoord.z;
    //gl_FragColor = vec4(vec3(gl_FragCoord.z*0.5 + 0.5), 1.0);
}
