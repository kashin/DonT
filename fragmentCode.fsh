uniform sampler2D texture;
varying mediump vec2 texc;

void main(void)
{
    gl_FragColor = texture2D(texture, texc);
}
