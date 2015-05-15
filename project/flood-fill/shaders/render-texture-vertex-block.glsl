attribute vec3 aPosition;
uniform mat4 uModelMatrix;
varying float vHeight;

void main(){
    gl_Position = uModelMatrix * vec4(aPosition, 1.0);
    vHeight = -gl_Position.z;
}
