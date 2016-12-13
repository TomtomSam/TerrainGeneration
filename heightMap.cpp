#include "heightMap.h"

using namespace std;

// GETTERS
int heightMap::getLength(){return length;}
int heightMap::getWidth(){return width;}
float heightMap::getMaxDepth(){return maxDepth;}
float heightMap::getMaxHeight(){return maxHeight;}
Point* heightMap::getHeightMap(int lig, int col){return heightMatrix[lig][col];}
vector<float> heightMap::getPos(){ return pos; }
vector<float> heightMap::getCol(){ return colors; }
vector<float> heightMap::getTex(){ return tex; }
float heightMap::getPosOcean(){ return posOcean; }
float heightMap::getDilatation(){ return dilatation; }
int heightMap::getTaille(){return pow(2, getLength());}

// SETTERS
void heightMap::setLength(int myLength){length=myLength;}
void heightMap::setWidth(int myWidth){width=myWidth;}
void heightMap::setMaxDepth(float myMaxDepth){maxDepth=myMaxDepth;}
void heightMap::setMaxHeight(float myMaxHeight){maxHeight=myMaxHeight;}
void heightMap::setHeightMap(int lig, int col, Point* point){heightMatrix[lig][col]=point;}
void heightMap::setTaille(){ taille = pow(2, getLength()); }
void heightMap::setMatrix(vector<Point*> newRow){ heightMatrix.push_back(newRow); }
void heightMap::clearMatrix(){ heightMatrix.clear(); }
void heightMap::resetDilatation(){ dilatation = 1.0f; }

void heightMap::setPosOcean(float _pos)
{
	float maxes[2];
	giveMaxes(maxes);
	//Limitation de l'océan entre les bornes de la map
	if (_pos<maxes[0] && _pos>maxes[1])
	{
		posOcean = _pos;
		FillDataBuffersColors();
	}
}

void heightMap::setDilatation(float _dilatation)
{
	if(_dilatation>1){dilatation = _dilatation;}
	else{dilatation = 1;}

	// Actualisation des datas
	FillDataBuffersPosColors();	
}

// METHODS
// Fonction d'initialisation manuelle de la map
void heightMap::initialisation(){
	float corner1,corner2,corner3,corner4; // Hauteur des coins
	int maxIndexColumn,maxIndexLine; // Derniere ligne/colonne de la map

	cout<<"Parameters of the Map:"<<endl;
	cout<<"Length: "; // Choix de la longueur de la map
	cin>>length;
	cout<<"Width: "; // Choix de la largeur de la map
	cin>>width;
	cout<<"Maximum depth: "; // Choix de l'altitude minimale de la map
	cin>>maxDepth;
	cout<<"Maximum height: "; // Choix de l'altitude maximale de la map
	cin>>maxHeight;

	do{ // Choix de l'altitude du coin en haut a gauche
		cout<<"Altitude of top left corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner1;
	}
	while((corner1>maxHeight)||(corner1<maxDepth));

	do{ // Choix de l'altitude du coin en haut a droite
		cout<<"Altitude of top right corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner2;
	}
	while((corner2>maxHeight)||(corner2<maxDepth));

	do{ // Choix de l'altitude du coin en bas a droite
		cout<<"Altitude of bottom right corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner3;
	}
	while((corner3>maxHeight)||(corner3<maxDepth));

	do{ // Choix de l'altitude du coin en bas a gauche
		cout<<"Altitude of bottom left corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner4;
	}
	while((corner4>maxHeight)||(corner4<maxDepth));

	cout<<endl;

	// Initialisation de maxIndexColumn et maxIndexLine
	maxIndexColumn=static_cast<int>(pow(2,length));
	maxIndexLine=static_cast<int>(pow(2,width));

	// Initialisation des hauteurs des coins
	setHeightMap(0,0,new Point(0,0,corner1));
	setHeightMap(0, maxIndexColumn, new Point(0, maxIndexColumn, corner2));
	setHeightMap(maxIndexLine, maxIndexColumn, new Point(maxIndexLine, maxIndexColumn, corner3));
	setHeightMap(maxIndexLine, 0, new Point(maxIndexLine, 0, corner4));
}

// Fonction d'initialisation automatique de la map
void heightMap::initialisationAuto(){
	int maxIndexColumn,maxIndexLine;// Derniere ligne/colonne de la map
	int signe = 0;

	maxIndexColumn = static_cast<int>(pow(2, length));
	maxIndexLine = maxIndexColumn;

	// Initialisation des hauteurs des coins a zero
	setHeightMap(0, 0, new Point(0, 0, 0));
	setHeightMap(0, maxIndexColumn, new Point(0, maxIndexColumn, 0));
	setHeightMap(maxIndexLine, maxIndexColumn, new Point(maxIndexLine, maxIndexColumn, 0));
	setHeightMap(maxIndexLine, 0, new Point(maxIndexLine, 0, 0));
}

// Etape diamant de l'algorithme de diamant-carre
void heightMap::diamondStep(int pas){
	const int h=static_cast<int>(pow(2,length));
	float moyenne=0.0;
	float random=0.0;
	int i=2*pas;

	for(int x=pas;x<=h;x+=i)
	{
		for(int y=pas;y<=h;y+=i)
		{
			moyenne+=heightMatrix[x-pas][y-pas]->getHeight();
			moyenne += heightMatrix[x-pas][y+pas]->getHeight();
			moyenne += heightMatrix[x+pas][y+pas]->getHeight();
			moyenne += heightMatrix[x+pas][y-pas]->getHeight();
			moyenne/=4;

			do{random=static_cast<float>((rand()%(2*pas+1))-pas);}
			while (abs(moyenne+random)>maxHeight);

			heightMatrix[x][y]->setHeight(moyenne+random);
			moyenne=0.0;
		}
	}
}

// Etape carre de l'algorithme de diamant-carre
void heightMap::squareStep(int pas){
	const int h=static_cast<int>(pow(2,length));
	float somme=0.0;
	int n=0;
	int decalage=0;
	float random=0.0;
	int i=2*pas;

	for(int x=0;x<=h;x+=pas)
	{
		if(x % i == 0)
		{
			decalage=pas;
		}
		else
		{
			decalage=0;
		}

		for(int y=decalage;y<=h;y+=i)
		{
			somme=0.0;
			n=0;
			if(x>=pas)
			{
				somme += heightMatrix[x-pas][y]->getHeight();
				n++;
			}
			if(x+pas<h)
			{
				somme += heightMatrix[x+pas][y]->getHeight();
				n++;
			}
			if(y>=pas)
			{
				somme += heightMatrix[x][y-pas]->getHeight();
				n++;
			}
			if(y+pas<h)
			{
				somme += heightMatrix[x][y+pas]->getHeight();
				n++;
			}

			do{random=static_cast<float>((rand() % (2*pas+1))-pas);}
			while (abs((somme / n) + random)>maxHeight);

			heightMatrix[x][y]->setHeight((somme/n)+random);
		}
	}
}

// Fonction d'implementation de l'algorithme de diamant carre
void heightMap::generateMatrix(){
	Chrono chrono;
	chrono.Tic();

	int i=static_cast<int>(pow(2,length));
	int pas ;

	while(i>1)
	{
		pas=i/2;
		diamondStep(pas);
		squareStep(pas);
		i=pas;
	}

	float maxes[2];
	giveMaxes(maxes);
	posOcean = static_cast<float>((maxes[0] - maxes[1])*0.3 + maxes[1]);

	chrono.Toc();
	cout << "Generation effectuee en " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s." << endl;
}

// Fonction de calcul des points culminant et inferieur
void heightMap::giveMaxes(float* max_min){
	float mini = maxHeight;
	float maxi = maxDepth;

	for (int a = 0; a <= pow(2, length); a++)
	{
		for (int b = 0; b <= pow(2, width); b++)
		{
			if (heightMatrix[a][b]->getHeight()>maxi)
			{
				maxi = heightMatrix[a][b]->getHeight();
			}
			if (heightMatrix[a][b]->getHeight()<mini)
			{
				mini = heightMatrix[a][b]->getHeight();
			}
		}
	}

	max_min[0]=maxi;
	max_min[1]=mini;
}

// Fonction de remplissage des couleurs de la map
void heightMap::mapColor()
{
	// Remplissage de la rampe de couleur quand on a toute les hauteurs
	float maxes[2];
	giveMaxes(maxes);
	laRampe.Remplissage(maxes[0], posOcean);

	int indice = 0;

	for (int l = 0; l<=pow(2, width); l++)
	{
		for (int c = 0; c<=pow(2, length); c++)
		{	
			// Sous l'ocean : couleur sable
			if (heightMatrix[l][c]->getHeight() < posOcean)
			{
				heightMatrix[l][c]->setR(laRampe.getR(0));
				heightMatrix[l][c]->setG(laRampe.getG(0));
				heightMatrix[l][c]->setB(laRampe.getB(0));
			}
			else
			{
				indice = (int)(heightMatrix[l][c]->getHeight() - posOcean);
				heightMatrix[l][c]->setR(laRampe.getR(indice));
				heightMatrix[l][c]->setG(laRampe.getG(indice));
				heightMatrix[l][c]->setB(laRampe.getB(indice));
			}	
		}
	}
}

// Fonction d'ecriture du fichier .obj associe a la map
void heightMap::ecrireFichierObj(){
	Chrono chrono;
	chrono.Tic();
	cout << "Ecriture du fichier OBJ, veuillez patienter..." << endl;

	int taille = static_cast<int>(pow(2, getLength()));
	int nbrPoint = static_cast<int>(pow(taille + 1, 2));
	ofstream myfile;
	myfile.open("map.obj",ios::out);
	myfile << "#map.obj\n";
	myfile << "#\n\n";
	myfile << "o Map\n\n";

	// Ecriture des vertex
	for (int i = 0; i <= taille; i++)
	{
		for (int j = 0; j <= taille; j++)
		{
			myfile << "v " << i*dilatation << " " << getHeightMap(i, j)->getHeight() *(1 + (dilatation - 1) / 3) << " " << j*dilatation << "\n";
		}
	}

	myfile << "\n";

	// Ecriture des facettes
	for (int j = 1; j < nbrPoint-taille-1; j++)
	{
		if (j % (taille+1) != 0)
		{
			myfile << "f " << j << " " << j + 1 << " " << j + taille + 1 << "\n";
		}
	}

	for (int j = 2; j < nbrPoint - taille; j++)
	{
		if (j % (taille + 1) != 1)
		{
			myfile << "f " << j << " " << j + taille + 1 << " " << j + taille << "\n";
		}
	}

	myfile.close();
	chrono.Toc();
	cout << "Fichier OBJ genere en " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s" << endl;
}

// Fonction de remplissage du VBO
void heightMap::FillDataBuffersPosColors()
{
	int taille = static_cast<int>(pow(2, length));
	float stride = 10.0f/taille;
	vector<float> tex1, tex2, tex3, tex4;
	int factor = 1/stride;

	//Sécutrité pour les map de taille 1,2 et 3.
	if (factor == 0){ factor = 1; }

	Chrono chrono;
	chrono.Tic();

	pos.clear();
	colors.clear();
	tex.clear();

	// Recalcul des couleurs de chaque point
	mapColor();

	// Remplissage strip par strip
	for (int i = 0; i < taille; i += 2)
	{

		// Remplissage des strips allant vers la droite
		for (int j = 0; j <= taille; j++)
		{
			// Les Positions 
			pos.push_back(i*dilatation);
			pos.push_back(heightMatrix[i][j]->getHeight());
			pos.push_back(j*dilatation);
			pos.push_back((i + 1)*dilatation);
			pos.push_back(heightMatrix[i + 1][j]->getHeight());
			pos.push_back(j*dilatation);

			// Les couleurs
			colors.push_back(heightMatrix[i][j]->getR());
			colors.push_back(heightMatrix[i][j]->getG());
			colors.push_back(heightMatrix[i][j]->getB());
			colors.push_back(heightMatrix[i+1][j]->getR());
			colors.push_back(heightMatrix[i+1][j]->getG());
			colors.push_back(heightMatrix[i+1][j]->getB());

			// Les textures
			tex1.push_back((i%factor)*stride);
			tex1.push_back((j%factor)*stride);
			tex1.push_back(((i+1)%factor)*stride);
			tex1.push_back((j%factor)*stride);
			tex2.push_back((i%factor)*stride);
			tex2.push_back((j%factor)*stride);
			tex2.push_back(((i+1)%factor)*stride);
			tex2.push_back((j%factor)*stride);
			tex3.push_back((i%factor)*stride);
			tex3.push_back((j%factor)*stride);
			tex3.push_back(((i+1)%factor)*stride);
			tex3.push_back((j%factor)*stride);
			tex4.push_back((i%factor)*stride);
			tex4.push_back((j%factor)*stride);
			tex4.push_back(((i+1)%factor)*stride);
			tex4.push_back((j%factor)*stride);
		}

		// Le dernier point de chaque strip est rentré deux fois dans le vecteur pour faire le virage
		// Remplissage des strips allant vers la gauche
		for (int j = taille; j >= 0; j--)
		{
			// Les Positions
			pos.push_back((i + 1)*dilatation);
			pos.push_back(heightMatrix[i + 1][j]->getHeight());
			pos.push_back(j*dilatation);
			pos.push_back((i + 2)*dilatation);
			pos.push_back(heightMatrix[i + 2][j]->getHeight());
			pos.push_back(j*dilatation);

			//Les couleurs
			colors.push_back(heightMatrix[i + 1][j]->getR());
			colors.push_back(heightMatrix[i + 1][j]->getG());
			colors.push_back(heightMatrix[i + 1][j]->getB());
			colors.push_back(heightMatrix[i + 2][j]->getR());
			colors.push_back(heightMatrix[i + 2][j]->getG());
			colors.push_back(heightMatrix[i + 2][j]->getB());

			// Les textures
			tex1.push_back(((i+1)%factor)*stride);
			tex1.push_back((j%factor)*stride);
			tex1.push_back(((i+2)%factor)*stride);
			tex1.push_back((j%factor)*stride);
			tex2.push_back(((i+1)%factor)*stride);
			tex2.push_back((j%factor)*stride);
			tex2.push_back(((i+2)%factor)*stride);
			tex2.push_back((j%factor)*stride);
			tex3.push_back(((i+1)%factor)*stride);
			tex3.push_back((j%factor)*stride);
			tex3.push_back(((i+2)%factor)*stride);
			tex3.push_back((j%factor)*stride);
			tex4.push_back(((i+1)%factor)*stride);
			tex4.push_back((j%factor)*stride);
			tex4.push_back(((i+2)%factor)*stride);
			tex4.push_back((j%factor)*stride);
		}
	}

	tex+=tex1;
	tex+=tex2;
	tex+=tex3;
	tex+=tex4;

	//Ajout du Cache Misère dans le vecteur de Pos
	
	//Répétition du dernier point de la map pour éviter de tracer le triangle indésirable de la strip
	pos.push_back(taille*dilatation);
	pos.push_back(heightMatrix[taille][0]->getHeight());
	pos.push_back(0);
	

	//Récupération de l'altitude minimale
	float maxes[2];
	giveMaxes(maxes);
	// En haut
	for (int i = taille; i >= 0; i--)
	{
		pos.push_back(i*dilatation);
		pos.push_back(maxes[1]);
		pos.push_back(0);
		if (getHeightMap(i, 0)->getHeight() >= posOcean)
		{
			pos.push_back(i*dilatation);
			pos.push_back(getHeightMap(i, 0)->getHeight());
			pos.push_back(0);
		}
		else
		{
			pos.push_back(i*dilatation);
			pos.push_back(posOcean);
			pos.push_back(0);
		}

	}
	// A droite
	for (int i = 0; i <= taille; i++)
	{
		pos.push_back(0);
		pos.push_back(maxes[1]);
		pos.push_back(i*dilatation);
		if (getHeightMap(0, i)->getHeight() > posOcean)
		{
			pos.push_back(0);
			pos.push_back(getHeightMap(0, i)->getHeight());
			pos.push_back(i*dilatation);
		}
		else
		{
			pos.push_back(0);
			pos.push_back(posOcean);
			pos.push_back(i*dilatation);
		}

	}
	// En bas
	for (int i = 0; i <= taille; i++)
	{
		pos.push_back(i*dilatation);
		pos.push_back(maxes[1]);
		pos.push_back(taille*dilatation);
		if (getHeightMap(i, taille)->getHeight() >= posOcean)
		{
			pos.push_back(i*dilatation);
			pos.push_back(getHeightMap(i, taille)->getHeight());
			pos.push_back(taille*dilatation);
		}
		else
		{
			pos.push_back(i*dilatation);
			pos.push_back(posOcean);
			pos.push_back(taille*dilatation);
		}

	}
	
	// A gauche
	for (int i = taille; i >= 0; i--)
	{
		pos.push_back(taille*dilatation);
		pos.push_back(maxes[1]);
		pos.push_back(i*dilatation);
		if (getHeightMap(taille, i)->getHeight() >= posOcean)
		{
			pos.push_back(taille*dilatation);
			pos.push_back(getHeightMap(taille, i)->getHeight());
			pos.push_back(i*dilatation);
		}
		else
		{
			pos.push_back(taille*dilatation);
			pos.push_back(posOcean);
			pos.push_back(i*dilatation);
		}

	}

	chrono.Toc();
	cout << "Remplissage des donnees effectue en: " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s." << endl;
}

// Fonction de remplissage des couleurs du VBO
void heightMap::FillDataBuffersColors()
{
	int taille = static_cast<int>(pow(2, length));

	colors.clear();

	// Recalcul les couleurs de chaque point
	mapColor();

	int j;

	// Remplissage strip par strip
	for (int i = 0; i < taille; i = i + 2)
	{
		// Remplissage des strips allant vers la droite
		for (j = 0; j <= taille; j++)
		{
			// Les couleurs
			colors.push_back(heightMatrix[i][j]->getR());
			colors.push_back(heightMatrix[i][j]->getG());
			colors.push_back(heightMatrix[i][j]->getB());
			colors.push_back(heightMatrix[i + 1][j]->getR());
			colors.push_back(heightMatrix[i + 1][j]->getG());
			colors.push_back(heightMatrix[i + 1][j]->getB());
		}
		// Le dernier point de chaque strip est rentré deux fois dans le vecteur pour faire le virage
		// Remplissage des strips allant vers la gauche
		for (j = taille; j >= 0; j--)
		{
			//Les couleurs
			colors.push_back(heightMatrix[i + 1][j]->getR());
			colors.push_back(heightMatrix[i + 1][j]->getG());
			colors.push_back(heightMatrix[i + 1][j]->getB());
			colors.push_back(heightMatrix[i + 2][j]->getR());
			colors.push_back(heightMatrix[i + 2][j]->getG());
			colors.push_back(heightMatrix[i + 2][j]->getB());
		}
	}

	//Nouvelle position du Cache Misère
	int nombreDePosMap = 3 * 2 * taille*(taille + 1);
	//Suppression des anciennes données du cache misère
	pos.erase(pos.begin() + nombreDePosMap, pos.end());

	//Repetition du dernier point de la map pour ne pas tracer le triangle indésirable de la Strip.
	pos.push_back(taille*dilatation);
	pos.push_back(heightMatrix[taille][0]->getHeight());
	pos.push_back(0);

	//Ré-écriture des données du cache misère
	//Récupération de l'altitude minimale
	float maxes[2];
	giveMaxes(maxes);
	// En haut
	for (int i = taille; i >= 0; i--)
	{
		pos.push_back(i*dilatation);
		pos.push_back(maxes[1]);
		pos.push_back(0);
		if (getHeightMap(i, 0)->getHeight() >= posOcean)
		{
			pos.push_back(i*dilatation);
			pos.push_back(getHeightMap(i, 0)->getHeight());
			pos.push_back(0);
		}
		else
		{
			pos.push_back(i*dilatation);
			pos.push_back(posOcean);
			pos.push_back(0);
		}

	}
	// A droite
	for (int i = 0; i <= taille; i++)
	{
		pos.push_back(0);
		pos.push_back(maxes[1]);
		pos.push_back(i*dilatation);
		if (getHeightMap(0, i)->getHeight() > posOcean)
		{
			pos.push_back(0);
			pos.push_back(getHeightMap(0, i)->getHeight());
			pos.push_back(i*dilatation);
		}
		else
		{
			pos.push_back(0);
			pos.push_back(posOcean);
			pos.push_back(i*dilatation);
		}

	}
	// En bas
	for (int i = 0; i <= taille; i++)
	{
		pos.push_back(i*dilatation);
		pos.push_back(maxes[1]);
		pos.push_back(taille*dilatation);
		if (getHeightMap(i, taille)->getHeight() >= posOcean)
		{
			pos.push_back(i*dilatation);
			pos.push_back(getHeightMap(i, taille)->getHeight());
			pos.push_back(taille*dilatation);
		}
		else
		{
			pos.push_back(i*dilatation);
			pos.push_back(posOcean);
			pos.push_back(taille*dilatation);
		}

	}

	// A gauche
	for (int i = taille; i >= 0; i--)
	{
		pos.push_back(taille*dilatation);
		pos.push_back(maxes[1]);
		pos.push_back(i*dilatation);
		if (getHeightMap(taille, i)->getHeight() >= posOcean)
		{
			pos.push_back(taille*dilatation);
			pos.push_back(getHeightMap(taille, i)->getHeight());
			pos.push_back(i*dilatation);
		}
		else
		{
			pos.push_back(taille*dilatation);
			pos.push_back(posOcean);
			pos.push_back(i*dilatation);
		}

	}

}

// Fonction de gestion du compteur FPS
void heightMap::compteurFPS(int windowW, int windowH, int FPS)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Couleur du texte
	if (FPS>55){ glColor3f(0.0f, 1.0f, 0.0f); }
	else if (FPS<56 && FPS>29){ glColor3f(1.0f, 0.5f, 0.0f); }
	else if (FPS<30){ glColor3f(1.0f, 0.0f, 0.0f); }

	// Positionnement du texte
	glRasterPos2i(10, windowH - 30);
	string s = to_string(FPS) + " FPS";

	// Choix Police
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;

	// Ecriture du texte
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}

// Fonction d'affichage de l'ocean
void heightMap::dessinOcean()
{
	int taille = static_cast<int>(pow(2, length));
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.75);
	glMultiTexCoord2f(GL_TEXTURE0 + 1,0,0);
	glMultiTexCoord2f(GL_TEXTURE0 + 4,0,0);
	glMultiTexCoord2f(GL_TEXTURE0 + 3,0,0);
	glMultiTexCoord2f(GL_TEXTURE0 + 2,0,0);
	glVertex3f(0, posOcean, 0);
	glMultiTexCoord2f(GL_TEXTURE0 + 1,1,0);
	glMultiTexCoord2f(GL_TEXTURE0 + 4,1,0);
	glMultiTexCoord2f(GL_TEXTURE0 + 3,1,0);
	glMultiTexCoord2f(GL_TEXTURE0 + 2,1,0);
	glVertex3f(taille*dilatation, posOcean, 0);
	glMultiTexCoord2f(GL_TEXTURE0 + 1,1,1);
	glMultiTexCoord2f(GL_TEXTURE0 + 4,1,1);
	glMultiTexCoord2f(GL_TEXTURE0 + 3,1,1);
	glMultiTexCoord2f(GL_TEXTURE0 + 2,1,1);
	glVertex3f(taille*dilatation, posOcean, taille*dilatation);
	glMultiTexCoord2f(GL_TEXTURE0 + 1,0,1);
	glMultiTexCoord2f(GL_TEXTURE0 + 4,0,1);
	glMultiTexCoord2f(GL_TEXTURE0 + 3,0,1);
	glMultiTexCoord2f(GL_TEXTURE0 + 2,0,1);
	glVertex3f(0, posOcean, taille*dilatation);
	glEnd();
}


// CONSTRUCTORS
heightMap::heightMap(){
	setLength(0);
	setWidth(0);
	setMaxDepth(0.0);
	setMaxHeight(0.0);
	dilatation = 1.0f;
	taille = pow(2, 5);

	for(int l=0;l<1024;l++) // Par defaut la map fait 1024x1024
	{
		vector<Point*> row;
		for(int c=0;c<1024;c++)
		{
			row.push_back(new Point(0,0,0));
		}
		heightMatrix.push_back(row);
	}
}

heightMap::heightMap(int size){

	if (size > 11){ size = 11; }
	if (size < 1){ size = 1; }

	setLength(size);
	setWidth(size);
	setMaxDepth(static_cast<float>(-pow(2, size)));
	setMaxHeight(static_cast<float>(pow(2, size)));
	dilatation = 1.0f;
	taille = pow(2, 5);

	for (int l = 0; l<1 + pow(2, size); l++)
	{
		vector<Point*> row;
		for (int c = 0; c<1 + pow(2, size); c++)
		{
			row.push_back(new Point(0, 0, 0));
		}
		heightMatrix.push_back(row);
	}
}

heightMap::heightMap(int myLength, int myWidth, float myMaxDepth, float myMaxHeight){
	setLength(myLength);
	setWidth(myWidth);
	setMaxDepth(myMaxDepth);
	setMaxHeight(myMaxHeight);
	dilatation = 1.0f;
	taille = pow(2, 5);

	for(int l=0;l<1+pow(2,myWidth);l++)
	{
		vector<Point *> row;
		for(int c=0;c<1+pow(2,myLength);c++)
		{
			row.push_back(new Point(0, 0, 0));
		}
		heightMatrix.push_back(row);
	}
}

// DESTRUCTOR
heightMap::~heightMap(){}