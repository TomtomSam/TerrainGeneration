varying float height;

void main()
{
	gl_TexCoord[1]  = gl_MultiTexCoord1;
	gl_TexCoord[2]  = gl_MultiTexCoord2;
	gl_TexCoord[3]  = gl_MultiTexCoord3;
	gl_TexCoord[4]  = gl_MultiTexCoord4;
	height = gl_Vertex.y;
	gl_Position = ftransform();
}
