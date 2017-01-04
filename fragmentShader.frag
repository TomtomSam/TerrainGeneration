uniform sampler2D tex_grass;
uniform sampler2D tex_ice;
uniform sampler2D tex_sand;
uniform sampler2D tex_snow;

uniform float maxHeight;
uniform float minHeight;
uniform float grassInf;
uniform float grassSup;
uniform float beachInf;
uniform float beachSup;
uniform float snowInf;
uniform float snowSup;
uniform float iceInf;

varying float height;
varying vec3 colorramp;
varying float texCoord1;
varying float texCoord2;

float contribution(float altitude, float borne_inf, float borne_sup)
{
	float result = 0;
	if((altitude>=borne_inf) && (altitude<=borne_sup)){result=(altitude-borne_inf)/(borne_sup-borne_inf);}
	return result;
}

void main()
{
	//Get Texture colors
	vec2 texcoord;
	texcoord.x = texCoord1;
	texcoord.y = texCoord2;
	vec3 grassColor	= texture2D(tex_grass,texcoord).rgb;
	vec3 iceColor	= texture2D(tex_ice,texcoord).rgb;
	vec3 sandColor = texture2D(tex_sand,texcoord).rgb;
	vec3 snowColor = texture2D(tex_snow,texcoord).rgb;
	
	//Calculate Texture Weights
	float grassWeight, iceWeight, sandWeight, snowWeight;
	
	sandWeight = contribution(height, beachInf, beachSup);
	grassWeight = contribution(height, grassInf, grassSup);
	snowWeight = contribution(height, snowInf, snowSup);
	iceWeight = contribution(height, iceInf, maxHeight);
	
	//Calculate Normalized Weights
	float totalWeight = grassWeight + sandWeight + iceWeight + snowWeight ;
	grassWeight = grassWeight/totalWeight;
	iceWeight = iceWeight/totalWeight;
	sandWeight = sandWeight/totalWeight;
	snowWeight = snowWeight/totalWeight;
	
	vec3 col = (grassWeight * grassColor.rgb + sandWeight * sandColor.rgb + iceWeight * iceColor.rgb + snowWeight * snowColor.rgb)*colorramp;

	gl_FragColor = vec4(col,1.0);
}