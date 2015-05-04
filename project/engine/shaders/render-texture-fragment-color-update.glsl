uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;
uniform vec2 uDTime;
uniform vec3 uStartPosition;

void main(){
    vec2 coord = gl_FragCoord.xy / 512.0;
    float r = min(texture2D(uDataTexture, coord).r, 1.0 - 1.0/uSize.y);
    vec2 increasePosition = vec2((uStartPosition.x / uSize.x + 1.0) / 2.0, (uStartPosition.z / uSize.z + 1.0)/2.0);
    gl_FragColor = vec4(0.0, 0.3, 0.9, clamp(uDTime.y * uDTime.y / dot(coord - increasePosition, coord - increasePosition), uDTime.y/1.5, 1.0));
}
