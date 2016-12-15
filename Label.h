#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <locale>

using namespace std;

class Label
{

private:
	int posX;
	int posY;
	int hauteur;
	int largeur;
	string label;

public:
	Label(int _posX, int _posY, string _label);
	~Label();

	void setLabel(string _label);
	string getLabel();
	int getLargeur();
	
	//Méthodes
	void draw(int windowW, int windowH);
	
};

