#include "Shader.h"

using namespace std;

Shader::Shader(void){}

Shader::~Shader(void){}

bool Shader::LoadShader(string sFile, int a_iType)
{
	vector<string> sLines;

	if(!GetLinesFromFile(sFile, false, &sLines))
		return false;

	const char** sProgram = new const char*[sLines.size()];
	for (int i = 0; i < int(sLines.size()); i++)
		sProgram[i] = sLines[i].c_str();
	
	shaderID = glCreateShader(a_iType);

	glShaderSource(shaderID, sLines.size(), sProgram, NULL);
	glCompileShader(shaderID);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)
	{
		char sInfoLog[1024];
		int iLogLength;
		glGetShaderInfoLog(shaderID, 1024, &iLogLength, sInfoLog);
		printf("Error! Shader file %s wasn't compiled!\n");
		return false;
	}

	type = a_iType;
	loaded = true;

	return true;
}

bool Shader::GetLinesFromFile(string sFile, bool bIncludePart, vector<string>* vResult)
{
	string inc = "#include_part";
	string def = "#definition_part";

	FILE* fp = fopen(sFile.c_str(), "rt");
	if(!fp)return false;

	string sDirectory;
	int slashIndex = -1;
	for (int i = sFile.size() - 1; i >= 0; i--)
	{
		if(sFile[i] == '\\' || sFile[i] == '/')
		{
			slashIndex = i;
			break;
		}
	}

	sDirectory = sFile.substr(0, slashIndex+1);

	// Get all lines from a file

	char sLine[255];

	bool bInIncludePart = false;

	while(fgets(sLine, 255, fp))
	{
		stringstream ss(sLine);
		string sFirst;
		ss >> sFirst;
		if(sFirst == "#include")
		{
			string sFileName;
			ss >> sFileName;
			if(sFileName.size() > 0 && sFileName[0] == '\"' && sFileName[sFileName.size()-1] == '\"')
			{
				sFileName = sFileName.substr(1, sFileName.size() -2);
				GetLinesFromFile(sDirectory+sFileName, true, vResult);
			}
		}
		else if(sFirst == inc)
			bInIncludePart = true;
		else if(sFirst == def)
			bInIncludePart = false;
		else if(!bIncludePart || (bIncludePart && bInIncludePart))
			vResult->push_back(sLine);
	}
	fclose(fp);

	return true;
}

bool Shader::IsLoaded()
{
	return loaded;
}

GLuint Shader::GetShaderID()
{
	return shaderID;
}

void Shader::DeleteShader()
{
	if(!IsLoaded()){loaded = false;}
	glDeleteShader(shaderID);
}
