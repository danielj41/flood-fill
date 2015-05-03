uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;
uniform vec2 uDTime;
uniform vec3 uStartPosition;

void main(){
    // http://madebyevan.com/webgl-water/water.js
    vec2 coord = gl_FragCoord.xy / 512.0;
    vec2 delta = 0.5 / uSize.xz;

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
    ) / 4.0;

    float vel = info.g * 2.0 - 1.0;

    if(info.r > 1.0/uSize.y) {
        vel -= uDTime.x/1.5;
    }

    vel += (average - info.r) * 20.0 * uDTime.x;

    vel += uDTime.x * -sign(vel);

    info.r += vel * 10.0 * uDTime.x;

    float amount = uDTime.y / 1.5;
    amount = amount * amount * amount;
    amount = min(amount, 1.0);

    info.r = (1.0 - amount) * info.r + amount * (1.0 / uSize.y);

    info.b = (1.0 - amount) * info.b + 0.5 * amount;
    info.a = (1.0 - amount) * info.a + 0.5 * amount;

     vec2 increasePosition = vec2((uStartPosition.x / uSize.x + 1.0) / 2.0, (uStartPosition.z / uSize.z + 1.0)/2.0);
    info.r += 1.0 * uDTime.x / (uDTime.y*uDTime.y*uDTime.y + 0.1) / uSize.y / (dot(coord - increasePosition, coord - increasePosition) / 0.05 + 1.0);

    info.g = (vel + 1.0) / 2.0;

   

    gl_FragColor = info;
}
