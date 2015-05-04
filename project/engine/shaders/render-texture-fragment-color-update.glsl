uniform sampler2D uPrevTexture;
uniform sampler2D uDataTexture;
uniform vec3 uSize;
uniform vec2 uDTime;
uniform vec3 uStartPosition;

void main(){
    vec2 coord = gl_FragCoord.xy / 512.0;
    if(texture2D(uDataTexture, coord).r < 0.99) {
        vec2 increasePosition = vec2((uStartPosition.x / uSize.x + 1.0) / 2.0, (uStartPosition.z / uSize.z + 1.0)/2.0);
        gl_FragColor = vec4(0.0, 0.3, 0.9, clamp(uDTime.y * 0.7 / dot(coord - increasePosition, coord - increasePosition), 0.0, 1.0));
    } else {
        gl_FragColor = vec4(0.0, 0.3, 0.9, 0.0);
    }
}
