uniform sampler2D tex_water;
uniform sampler2D tex_grass;
uniform sampler2D tex_ice;
uniform sampler2D tex_sand;

uniform int waterID;
uniform int grassID;
uniform int iceID;
uniform int sandID;

uniform float maxHeight;
uniform float minHeight;
uniform float grassInf;
uniform float grassSup;
uniform float beachInf;
uniform float beachSup;
uniform float snowInf;
uniform float waterSup;

varying float height;

float contribution(float altitude, float borne_inf, float borne_sup)
{
	float result = 0;
	if((altitude>=borne_inf) && (altitude<=borne_sup)){result=(altitude-borne_inf)/(borne_sup-borne_inf);}
	return result;
}

void main()
{
	//Get Texture colors
	vec3 waterColor	= texture2D(tex_water,gl_TexCoord[waterID].st).rgb;
	vec3 grassColor	= texture2D(tex_grass,gl_TexCoord[grassID].st).rgb;
	vec3 iceColor	= texture2D(tex_ice,gl_TexCoord[iceID].st).rgb;
	vec3 sandColor	= texture2D(tex_sand,gl_TexCoord[sandID].st).rgb;
	
	//Calculate Texture Weights
	float waterWeight, grassWeight, iceWeight, sandWeight;
	
	waterWeight = contribution(height, minHeight, waterSup);
	if(waterWeight>0){sandWeight=0;}
	else{sandWeight = contribution(height, beachInf, beachSup);}
	grassWeight = contribution(height, grassInf, grassSup);
	iceWeight = contribution(height, snowInf, maxHeight);
	
	//Calculate Normalized Weights
	float totalWeight = waterWeight + grassWeight + sandWeight + iceWeight ;
	waterWeight= waterWeight/totalWeight;
	grassWeight = grassWeight/totalWeight;
	iceWeight = iceWeight/totalWeight;
	sandWeight = sandWeight/totalWeight;
	
	gl_FragColor = vec4((waterWeight * waterColor.rgb + grassWeight * grassColor.rgb + sandWeight * sandColor.rgb + iceWeight * iceColor.rgb),1.0);
}

