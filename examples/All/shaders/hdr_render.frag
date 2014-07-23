uniform sampler2D texture;
uniform vec2 range;

void main(void)
{
    vec4 color = texture2D(texture, gl_TexCoord[0].st);

	color-= range.x;
	color/= range.y-range.x;

    gl_FragColor = color;
}

