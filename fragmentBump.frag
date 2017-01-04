uniform sampler2D bumptex;
uniform float bumpID;

void main()
{
	vec3 waterColor	= texture2D(bumptex, gl_TexCoord[1].st).rgb;
	gl_FragColor = vec4(waterColor.rgb, 1.0);
}
