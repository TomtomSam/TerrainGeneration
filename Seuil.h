#ifndef SEUIL_H
#define SEUIL_H

class Seuil
{
public:
	Seuil(void);
	~Seuil(void);
	void setHauteur(float _hauteur);
	float getHauteur(void);
	Seuil operator++(int); // surcharge de l'operateur d'incrementation suffixee
	Seuil operator--(int); // surcharge de l'operateur de decrementation suffixee

private:
	float hauteur;
};



#endif // SEUIL_H