#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <locale>

using namespace std;

class Bouton
{
private:
	int posX;
	int posY;
	int hauteur;
	int largeur;
	bool survol;
	string label;
	

public:
	Bouton(int _posX,int _posY, string _label);
	~Bouton();


	bool getSurvol();
	int getLargeur();
	//Méthodes
	void draw(int windowW, int windowH);
	bool isMouseInMe(int x, int y, int windowW, int windowH);

};

