#version 130
#extension GL_ARB_explicit_attrib_location: require

layout(location = 0) out float depth;

void main(){
    depth = gl_FragCoord.z;
}
