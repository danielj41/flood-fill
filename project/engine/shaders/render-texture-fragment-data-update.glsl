uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;

void main(){
    // http://madebyevan.com/webgl-water/water.js
    vec2 coord = gl_FragCoord.xy / 256.0;
    vec2 delta = vec2(0.05, 0.05);

    vec4 info = texture2D(uPrevTexture, coord);
    vec3 dx = vec3(delta.x, texture2D(uPrevTexture, vec2(coord.x + delta.x, coord.y)).r - info.r, 0.0);
    vec3 dy = vec3(0.0, texture2D(uPrevTexture, vec2(coord.x, coord.y + delta.y)).r - info.r, delta.y);
    info.ba = (normalize(cross(dy, dx)).xz + vec2(1.0, 1.0)) / 2.0;

    vec2 dx2 = vec2(delta.x, 0.0);
    vec2 dy2 = vec2(0.0, delta.y);
    float average = (
        texture2D(uPrevTexture, coord - dx2).r +
        texture2D(uPrevTexture, coord - dy2).r +
        texture2D(uPrevTexture, coord + dx2).r +
        texture2D(uPrevTexture, coord + dy2).r
    ) * 0.25;

    float vel = info.g * 2.0 - 1.0;

    vel += (average - info.r) * 0.10;

    vel *= 0.995;

    info.r += vel;

    info.g = (vel + 1.0) / 2.0;

    gl_FragColor = info;
}
