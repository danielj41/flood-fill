uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;
uniform vec3 uStartPosition;

void main(){
    vec2 coord = gl_FragCoord.xy / 256.0;
    vec2 increasePosition = vec2((uStartPosition.x / uSize.x + 1.0) / 2.0, (uStartPosition.z / uSize.z + 1.0)/2.0);
    gl_FragColor = vec4(clamp(1.0 / (dot(coord - increasePosition, coord - increasePosition) + 1.0), 0.0, 1.0) * min(texture2D(uDataTexture, gl_FragCoord.xy / 256.0).r - 0.5/uSize.y, 1.0 - 1.0/uSize.y), 0.5, 0.0, 1.0);
}
