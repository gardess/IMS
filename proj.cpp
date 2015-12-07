#include <iostream>
#include <simlib.h>

#define MINUTA 60
#define PROVOZNIDOBA 12600

Facility fac1("Pult s pitim");
Facility fac2("Pult polevky+hlavni jidlo 1");
Facility fac3("Pult s hlavnim jidlem 2");
Facility pokladna("Pokladna 1");

Store pocetMist("Pocet zidli", 120);

Queue pult2Q("Pult 2 Fronta");


Stat dobaObsluhyPult2("Doba obsluhy u pultu 2");

int J2Pi = 0;
int J2 = 0;
int Pi = 0;
int J1 = 0;
int J1Pi = 0;
//int P = 0; // polevka
int PJ1 = 0;
int PJ1Pi = 0;
int PJ2 = 0;
int PJ2Pi = 0;
int celkemStudentu = 0;

class Student : public Process
{
	void Behavior()
	{
		double prav = Random();
		if (prav <= 0.33)
		{
			Seize(fac2);
			double polevkaPrav = Random();
			if (polevkaPrav <= 0.77) // beru si polevku
			{
				Wait(Exponential(5));
			}

			double jidloPrav = Random();
			if (jidloPrav <= 0.38) // beru hlavni jidlo z polevkoveho pultu (buchty apod.)
			{
				Wait(Uniform(5, 15));
			}
			else
			{
				Priority = 1;
				Release(fac2);
				goto HlavniJidlo2;
			}
			Release(fac2);
			double voda1 = Random();
			if (voda1 <= 0.50)
			{
				goto Piti;
			}
			goto Pokladna;
		}
		else
		{
HlavniJidlo2:
			Seize(fac3);
			Priority = 0;
			double priprava = Random();
			if (priprava <= 0.25) // jidlo je nachystano a muzeme si ho ihned vzit
			{
				Wait(Exponential(5));
			}
			else // rikame co chceme a cekame na pozadovane jidlo
			{
				Wait(Uniform(10, 15));
			}
			Release(fac3);
			double voda = Random();
			if (voda <= 0.50)
			{
				goto Piti;
			}
			goto Pokladna;
		}

Piti:
	Seize(fac1);
	Wait(Exponential(5));
	Release(fac1);

Pokladna:
		Seize(pokladna); 
		if (Random() > 0.15) // student ma dostatek penez na konte
		{
			Wait(Exponential(7));
		}
		else
		{
			Wait(Uniform(10, 15));
		}

		Release(pokladna);

		Enter(pocetMist, 1); // zabrani zidle

		Wait(Uniform(9 * MINUTA, 20 * MINUTA)); // obedvani

		Leave(pocetMist, 1); // vraceni zidle a opusteni menzy
	}
}; // konec tridy student

class Prichod : public Event
{
	void Behavior()
	{
		(new Student)->Activate();
		/* Cas aktivace, bude se menit (cela podstata experimentů) */
		Activate(Time+Exponential(12));
	}
	
};

int main()
{
	//std::cout << "Hello world" << std::endl;
	//Init(0, 60 * MINUTA);
	Init(0, PROVOZNIDOBA);
	(new Prichod)->Activate();
	RandomSeed(time(NULL));
	Run();

	fac1.Output();
	fac2.Output();
	fac3.Output();
	pokladna.Output();
	pocetMist.Output();
	//dobaObsluhyPult2.Output();
	std::cout << "Pouze piti:	" << Pi << std::endl;
	std::cout << "Pouze jidlo 2:	" << J2 << std::endl;
	std::cout << "Jidlo 2 + Piti:	" << J2Pi << std::endl;
	std::cout << "Pouze jidlo 1:	" << J1 << std::endl;
	std::cout << "Jidlo 1 + Piti:	" << J1Pi << std::endl;
	std::cout << "Polevka + Jidlo 1:	" << PJ1 << std::endl;
	std::cout << "Polevka + Jidlo 1 + Piti:	" << PJ1Pi << std::endl;
	std::cout << "Polevka + Jidlo 2:	" << PJ2 << std::endl;
	std::cout << "Polevka + Jidlo 2 + Piti:	" << PJ2Pi << std::endl;
	std::cout << "Celkem Studentu:	" << celkemStudentu << std::endl;
/*int Pi = 0;
int J2 = 0;
int J2Pi = 0;
int J1 = 0;
int J1Pi = 0;
int PJ1 = 0;
int PJ1Pi = 0;
int PJ2 = 0;
int PJ2Pi = 0;*/
}