varying float height;
varying vec3 colorramp;
varying float texCoord1;
varying float texCoord2;

uniform float taille;

void main()
{
	texCoord1 = gl_Vertex.x/taille;//10*gl_Vertex.x/taille - floor(10*gl_Vertex.x/taille);
	texCoord2 = gl_Vertex.z/taille;//10*gl_Vertex.z/taille - floor(10*gl_Vertex.z/taille);
	height = gl_Vertex.y;
	colorramp = gl_Color.rgb;
	gl_Position = ftransform();
}

