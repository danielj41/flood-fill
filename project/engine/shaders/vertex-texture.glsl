#version 130

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

out vec3 vVertex;
flat out vec3 vNormal;
out vec2 vTexCoord;

void main(){

    vNormal = vec3(normalize(uNormalMatrix*vec4(aNormal, 0)));
    vVertex = vec3(uProjection*uView*uModel*vec4(aPosition, 1));
    vTexCoord = aTexCoord;

    gl_Position = uProjection*uView*uModel*vec4(aPosition, 1);
}
