attribute vec3 position;
attribute vec2 texcoord;

uniform mat4 uProjection;
uniform mat4 uView;

varying vec2 texpos;

void main(void) {
       mat4 pos = mat4(1.0,0,0,0,
                       0,1.0,0,0,
                       0,0,1.0,0,         
                       position.x, position.y, position.z, 1.0);
     
    gl_Position = uProjection * uView * pos * vec4(1,1,1,1);
    texpos = texcoord;
}