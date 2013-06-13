uniform sampler2D texture;

void main(void)
{
    vec4 color = texture2D(texture, gl_TexCoord[0].st);
    gl_FragColor = color;
}

