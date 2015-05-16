#version 120

void main(){
    gl_FragDepth = gl_FragCoord.z;
    gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    //gl_FragColor = vec4(vec3(gl_FragCoord.z), 1);
}
