//attribute vec3 tangent;
//attribute vec3 binormal;

void main() 
{ 
     gl_TexCoord[1] = gl_MultiTexCoord1; 
     gl_Position = ftransform(); 
}
