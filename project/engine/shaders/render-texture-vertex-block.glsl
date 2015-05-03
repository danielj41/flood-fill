attribute vec3 aPosition;
attribute vec3 aNormal;
uniform sampler2D uPrevTexture;
uniform mat4 uModelMatrix;
varying float vHeight;

void main(){
    aNormal;
    uPrevTexture;
    gl_Position = uModelMatrix * vec4(aPosition, 1.0);
    vHeight = gl_Position.z;
}
