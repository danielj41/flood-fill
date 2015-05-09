uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;
uniform vec2 uDTime;
uniform vec3 uStartPosition;

void main(){
    vec2 coord = gl_FragCoord.xy / 256.0;
    vec2 delta = 0.05 / uSize.xz;

    vec4 info = texture2D(uPrevTexture, coord);
    vec4 block = texture2D(uDataTexture, coord);
    
    vec3 dx = vec3(delta.x * 5.0, texture2D(uPrevTexture, vec2(coord.x + delta.x, coord.y)).r - info.r, 0.0);
    vec3 dy = vec3(0.0, texture2D(uPrevTexture, vec2(coord.x, coord.y + delta.y)).r - info.r, delta.y * 5.0);
    info.ba = (normalize(cross(dy, dx)).xz + vec2(1.0, 1.0)) / 2.0;

    vec2 dx2 = vec2(delta.x, 0.0);
    vec2 dy2 = vec2(0.0, delta.y);

    float average = (
        texture2D(uPrevTexture, coord - dx2).r - 0.5*min(texture2D(uDataTexture, coord - dx2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord - dy2).r - 0.5*min(texture2D(uDataTexture, coord - dy2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord + dx2).r - 0.5*min(texture2D(uDataTexture, coord + dx2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord + dy2).r - 0.5*min(texture2D(uDataTexture, coord + dy2).r, 1.0 - 1.0/uSize.y) +

        texture2D(uPrevTexture, coord - 4.0*dx2).r - 0.5*min(texture2D(uDataTexture, coord - 4.0*dx2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord - 4.0*dy2).r - 0.5*min(texture2D(uDataTexture, coord - 4.0*dy2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord + 4.0*dx2).r - 0.5*min(texture2D(uDataTexture, coord + 4.0*dx2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord + 4.0*dy2).r - 0.5*min(texture2D(uDataTexture, coord + 4.0*dy2).r, 1.0 - 1.0/uSize.y) + 

        texture2D(uPrevTexture, coord - 16.0*dx2).r - 0.5*min(texture2D(uDataTexture, coord - 16.0*dx2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord - 16.0*dy2).r - 0.5*min(texture2D(uDataTexture, coord - 16.0*dy2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord + 16.0*dx2).r - 0.5*min(texture2D(uDataTexture, coord + 16.0*dx2).r, 1.0 - 1.0/uSize.y) +
        texture2D(uPrevTexture, coord + 16.0*dy2).r - 0.5*min(texture2D(uDataTexture, coord + 16.0*dy2).r, 1.0 - 1.0/uSize.y)
    ) / 12.0;

    float vel = info.g * 2.0 - 1.0;

    if(info.r > 1.0/uSize.y) {
        vel -= uDTime.x/1.5;
    }

    block.r = min(block.r,  1.0 - 1.0/uSize.y);

    vel += (average - info.r + 0.5 * block.r) * 50.0 * uDTime.x;

    vec2 increasePosition = vec2((uStartPosition.x / uSize.x + 1.0) / 2.0, (uStartPosition.z / uSize.z + 1.0)/2.0);
    vel += 1.5 * uDTime.x / (uDTime.y + 1.0) / (uDTime.y + 1.0) / (dot(coord - increasePosition, coord - increasePosition) / 0.15 + 2.00);

    vel += (block.r + 0.3/uSize.y - info.r) * 10.0 * uDTime.x;

    vel *= pow(0.995, uDTime.x * 60.0);

    vel = clamp(vel, -1.0, 1.0);

    float height = info.r;

    height += vel * 5.0 * uDTime.x;

    float amount = uDTime.y / 1.5;
    amount = amount * amount * amount;
    amount = min(amount, 1.0);
    
    height = max(height, (1.0 - amount) * (block.r + 0.3/uSize.y));

    info.b = (1.0 - amount) * info.b + 0.5 * amount;
    info.a = (1.0 - amount) * info.a + 0.5 * amount;

    info.g = (vel + 1.0) / 2.0;
    info.r = height;

    gl_FragColor = info;
}
