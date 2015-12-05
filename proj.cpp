#include <iostream>
#include <simlib.h>

#define MINUTA 60
#define PROVOZNIDOBA 12600

Facility fac1("Pult s pitim");
Facility fac2("Pult polevky+hlavni jidlo 1");
Facility fac3("Pult s hlavnim jidlem 2");
Facility pokladna("Pokladna 1");

Store pocetMist("Pocet zidli", 120);

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
		//double prichod = Time;
		double obsluha;
		celkemStudentu++;
		 if (Random() <= 0.0025)
		 {
		 	//student bere pouze piti
		 	Seize(fac1);
		 	Wait(Exponential(5));
		 	Release(fac1);
		 	Pi++;

		 	// Pokladna
		 	Seize(pokladna);
		 	if (Random() <= 0.75)
		 	{
		 		Wait(Exponential(7));
		 	}
		 	else
		 	{
		 		Wait(Exponential(25));
		 	}
		 	Release(pokladna);

		 	// usazeni a jezeni obědu
		 	Enter(pocetMist, 1);
		 	Wait(Uniform(9 * MINUTA, 20 * MINUTA));
		 	Leave(pocetMist, 1);
		 }

		else if (Random() > 0.2025)
		 {
		 	// student bere hlavni jidlo od pult c.2
		 	Seize(fac3);

		 	if (Random() <= 0.90) // na pultu neni pripravene jidlo ktere chce
		 	{
		 		obsluha = Exponential(15);
		 		Wait(obsluha);
		 		dobaObsluhyPult2(obsluha);
		 		//Wait(Exponential(15));
		 	}
		 	else
		 	{
		 		obsluha = Exponential(5);
		 		Wait(obsluha);
		 		dobaObsluhyPult2(obsluha);
		 		//Wait(Exponential(5)); // muze okamzite odebrat jidlo
		 	}
		 	Release(fac3);
		 	J2++;
		 	if(Random() <= 0.5)
		 	{
		 		// Student chce i piti
		 		Seize(fac1);
		 		Wait(Exponential(5));
		 		Release(fac1);
		 		J2Pi++;

		 	}

		 	// Pokladna
		 	Seize(pokladna);
		 	if (Random() <= 0.75)
		 	{
		 		Wait(Exponential(7));
		 	}
		 	else
		 	{
		 		Wait(Exponential(25));
		 	}
		 	Release(pokladna);

		 	// usazeni a jezeni obědu
		 	Enter(pocetMist, 1);
		 	Wait(Uniform(9 * MINUTA, 20 * MINUTA));
		 	Leave(pocetMist, 1);
		 
		 }
/////////////////
		 else
		 {
		 	//student jde k polevkovemu pultu
		 	Seize(fac2);
		 	if (Random() <= 0.75) //student bere polevku
		 	{
		 		if (Random() <= 0.90)
		 		{
		 			// polevka je pripravena ke vziti
		 			Wait(Exponential(5));
		 		}
		 		else
		 		{
		 			// polevka se musi nalet do misky
		 			Wait(Exponential(10));
		 		}

		 		// volba zda student chce hlavni jidlo z pultu 1 nebo 2
		 		if (Random() <= 0.33)
		 		{
		 			// hlavni jidlo z pultu c.1 (Pult polevky+hlavni jidlo 1)
		 			if (Random() <= 0.85)
		 			{
		 				// jidlo neni pripraveno
		 				Wait(Exponential(15));
		 			}
		 			else
		 			{
		 				// jidlo je pripraveno
		 				Wait(Exponential(5));
		 			}
		 			Release(fac2);
		 			PJ1++;
		 			// rozhodovani zde student chce i piti
		 			if(Random() <= 0.5)
				 	{
				 		// Student chce i piti
				 		Seize(fac1);
				 		Wait(Exponential(5));
				 		Release(fac1);
				 		PJ1Pi++;
				 	}

		 			// Pokladna
				 	Seize(pokladna);
				 	if (Random() <= 0.75)
				 	{
				 		Wait(Exponential(7));
				 	}
				 	else
				 	{
				 		Wait(Exponential(25));
				 	}
				 	Release(pokladna);

				 	// usazeni a jezeni obědu
				 	Enter(pocetMist, 1);
				 	Wait(Uniform(9 * MINUTA, 20 * MINUTA));
				 	Leave(pocetMist, 1);

		 		} // konec moznosti Polevka + Hlavni jidlo z pultu c.1
		 		else
		 		{
		 			// hlavni jidlo z pultu c.2 (Pult polevky+hlavni jidlo 1)
		 			Release(fac2);
		 			Seize(fac3);
		 			if (Random() <= 0.90)
		 			{
		 				// jidlo neni pripraveno
		 				//Wait(Exponential(15));
		 				obsluha = Exponential(15);
				 		Wait(obsluha);
				 		dobaObsluhyPult2(obsluha);
		 			}
		 			else
		 			{
		 				// jidlo je pripraveno
		 				//Wait(Exponential(5));
		 				obsluha = Exponential(5);
				 		Wait(obsluha);
				 		dobaObsluhyPult2(obsluha);
		 			}
		 			Release(fac3);
		 			PJ2++;
		 			// rozhodovani zde student chce i piti
		 			if(Random() <= 0.5)
				 	{
				 		// Student chce i piti
				 		Seize(fac1);
				 		Wait(Exponential(5));
				 		Release(fac1);
				 		PJ2Pi++;
				 	}

		 			// Pokladna
				 	Seize(pokladna);
				 	if (Random() <= 0.75)
				 	{
				 		Wait(Exponential(7));
				 	}
				 	else
				 	{
				 		Wait(Exponential(25));
				 	}
				 	Release(pokladna);

				 	// usazeni a jezeni obědu
				 	Enter(pocetMist, 1);
				 	Wait(Uniform(9 * MINUTA, 20 * MINUTA));
				 	Leave(pocetMist, 1);

		 		} //konec moznosti Polevka + Hlavni jidlo z pultu c.2 (Pult s hlavnim jidlem 2)

		 	} // konec moznosti pri ktere student bere polevku

		 	else
		 	{
		 		// pouze hlavni jidlo z pultu c.1 (Pult polevky+hlavni jidlo 1) tj. NEBERE POLEVKU!!!
		 		if (Random() <= 0.85)
		 		{
		 			// jidlo neni pripraveno
		 			Wait(Exponential(15));
		 		}
		 		else
		 		{
		 			// jidlo je pripraveno
		 			Wait(Exponential(5));
		 		}
		 		Release(fac2);
		 		J1++;
		 		// rozhodovani zde student chce i piti
		 		if(Random() <= 0.5)
				{
					// Student chce i piti
					Seize(fac1);
					Wait(Exponential(5));
					Release(fac1);
					J1Pi++;
				}

		 		// Pokladna
			 	Seize(pokladna);
			 	if (Random() <= 0.75)
			 	{
			 		Wait(Exponential(7));
			 	}
			 	else
			 	{
			 		Wait(Exponential(25));
			 	}
			 	Release(pokladna);

			 	// usazeni a jezeni obědu
			 	Enter(pocetMist, 1);
			 	Wait(Uniform(9 * MINUTA, 20 * MINUTA));
			 	Leave(pocetMist, 1);

		 	} // konec moznosti pouze Hlavni jidlo z pultu 1 (Pult polevky+hlavni jidlo 1)

		 }
		 
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
	Init(0, 60*60*3.5);
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