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
uniform float oceanTransition;
uniform float beachTransition;
uniform float snowTransition;

varying float height;

void main()
{
	//Get Texture colors
	vec3 waterColor	= texture2D(tex_water,gl_TexCoord[waterID].st).rgb;
	vec3 grassColor	= texture2D(tex_grass,gl_TexCoord[grassID].st).rgb;
	vec3 iceColor	= texture2D(tex_ice,gl_TexCoord[iceID].st).rgb;
	vec3 sandColor	= texture2D(tex_sand,gl_TexCoord[sandID].st).rgb;
	
	//Calculate Texture Weights
	float waterWeight, grassWeight, iceWeight, sandWeight;
	
	if(height>oceanTransition){waterWeight=0;}
	else{waterWeight=(height-minHeight)/(oceanTransition-minHeight);}
	
	if((height>beachTransition) ||(height<oceanTransition)){sandWeight=0;}
	else{sandWeight=(height-minHeight)/(beachTransition-minHeight);}
	
	if((height>snowTransition) ||(height<beachTransition)){grassWeight=0;}
	else{grassWeight=(height-minHeight)/(snowTransition-minHeight);}
	
	if(height<snowTransition){iceWeight=0;}
	else{iceWeight=(height-minHeight)/(maxHeight-minHeight);}
	
	//Calculate Normalized Weights
	/*float totalWeight = waterWeight + grassWeight + sandWeight + iceWeight ;
	waterWeight= waterWeight /totalWeight;
	grassWeight = grassWeight/totalWeight;
	iceWeight = iceWeight/totalWeight;
	sandWeight = sandWeight/totalWeight;*/
	
	gl_FragColor = vec4((waterWeight * waterColor.rgb + grassWeight * grassColor.rgb + sandWeight * sandColor.rgb + iceWeight * iceColor.rgb),1.0);
}

