#ifndef CHRONO_H
#define CHRONO_H

class Chrono
{
public:
	Chrono(void);
	virtual ~Chrono(void);
	void Tic();
	void Toc();
	int getEllapsed_time();

private:
	int current_time;
	int last_time;
	int ellapsed_time;
};

#endif // CHRONO_H
