/* Projekt IMS - Model studentské menzy
 * Autori:	Gardas Milan (xgarda04@stud.fit.vutbr.cz) - zastupce tymu
 * 			Gerguri Denis (xgergu01@stud.fit.vutbr.cz)
 * Zakladni casova jednotka je 1 sekunda
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <simlib.h>

#define MINUTA 60
#define HODINA 3600
#define PROVOZNIDOBA 12600
#define DEBUG 0

using std::cout;
using std::endl;

// promenne pro parametry
std::string filename = "Menza.out";
long paramPokladny = 1;
long paramPultHlavniJidlo = 1;
double pripravaPrav = 0.25;
long paramOptimalizace = 0; 

int pocetS = 1;

Queue pultFronta("Spolecna fronta");

// pulty s jidlem nebo pitim
Facility fac1("Pult s pitim");
Facility fac2("Pult polevky+hlavni jidlo 1");
Facility fac3("Pult s hlavnim jidlem 2");
Facility fac4("Pult s hlavnim jidlem 2 pridany parametrem");


// pokladny
Facility pokladna1("Pokladna 1");
Facility pokladna2("Pokladna 2 (pridana parametrem)");

// mista u stolu v menze
Store pocetMist("Pocet zidli", 120);


Histogram pokladna1H("Pocet studentu, ktery opustili pokladnu 1", 0, MINUTA, 210);
Histogram pokladna2H("Pocet studentu, ktery opustili pokladnu 2", 0, MINUTA, 210);
Histogram obed1H("Pocet studentu, ktery maji hlavni jidlo z pultu 1", 0, MINUTA, 210);
Histogram obed2H1("Pocet studentu, ktery maji hlavni jidlo z pultu 2.1", 0, MINUTA, 210);
Histogram polevkaH("Pocet studentu, ktery si vzali polevku", 0, MINUTA, 210);
Histogram prichodH("Casy prichodu studentu", 0, MINUTA, 210);
Histogram obed2H2("Pocet studentu, ktery maji hlavni jidlo z pultu 2.2", 0, MINUTA, 210);


class Student : public Process
{

	void Behavior()
	{
		prichodH(Time);
		double prav = Random();

		if (prav <= 0.33) // chci polevku nebo hlavni jidlo typu buchty apod.
		{
			Seize(fac2);
			double polevkaPrav = Random();
			if (polevkaPrav <= 0.77) // beru si polevku
			{
				Wait(Exponential(5));
				polevkaH(Time); // ulozeni hodnoty do histogramu
			}

			double jidloPrav = Random();
			if (jidloPrav <= 0.38) // beru hlavni jidlo z polevkoveho pultu (buchty apod.)
			{
				double priprava1 = Random();
				if (priprava1 <= pripravaPrav) // jidlo je nachystano a muzeme si ho ihned vzit
				{
					Wait(Exponential(5));
				}
				else // rikame co chceme a cekame na pozadovane jidlo
				{
					Wait(Uniform(10, 15));
				}
			}
			else // chci jidlo z pultu na Hlavni jidla a všechny předběhnu (az na ty co maji stejne chute a prisli predemnou) 
			{
				Priority = 1;
				Release(fac2);
				goto HlavniJidlo2;
			}
			obed1H(Time); // ulozeni hodnoty do histogramu
			Release(fac2);
			double voda1 = Random();
			if (voda1 <= 0.50) // chci si koupit neco na piti
			{
				goto Piti;
			}
			// jdu rovnou k pokladne
			goto Pokladna;
		} // konec Pult polevky+hlavni jidlo 1

		else
		{
HlavniJidlo2:
			int pult = 0;
			if (paramPultHlavniJidlo == 2)
			{
				if (fac3.QueueLen() <= fac4.QueueLen())
				{
					Seize(fac3);
					pult = 1;
				}
				else
				{
					Seize(fac4);
					pult = 2;
				}
			}
			else
			{
				Seize(fac3);
			}
			Priority = 0;
			double priprava2 = Random();
			if (priprava2 <= pripravaPrav) // jidlo je nachystano a muzeme si ho ihned vzit
			{
				Wait(Exponential(5));
			}
			else // rikame co chceme a cekame na pozadovane jidlo
			{
				Wait(Uniform(10, 15));
			}
			if (paramPultHlavniJidlo == 2)
			{
				if (pult == 1)
				{
					obed2H1(Time);// ulozeni hodnoty do histogramu
					Release(fac3);
				}
				else
				{
					obed2H2(Time);
					Release(fac4);
				}
			}
			else
			{
				obed2H1(Time); // ulozeni hodnoty do histogramu
				Release(fac3);
			}
			double voda = Random();
			if (voda <= 0.50)
			{
				goto Piti;
			}
			goto Pokladna;
		} // konec Pult s hlavnim jidlem 2 

Piti: // chci neco na piti
	Seize(fac1);
	Wait(Exponential(5));
	Release(fac1);

Pokladna: // platba, zabrani zidle a samotne jezeni
		int pok = 0; 
		if (paramPokladny == 2)
		{
			if (pokladna1.QueueLen() < pokladna2.QueueLen()) // vyber pokladny podle delky fronty
			{
				Seize(pokladna1);
				pok = 1;
			}
			else if (pokladna1.QueueLen() == pokladna2.QueueLen())
			{
				if (Random() <= 0.5)
				{
					Seize(pokladna1);
					pok = 1;
				}
				else
				{
					Seize(pokladna2);
					pok = 2;
				}
			}
			else
			{
				Seize(pokladna2);
				pok = 2;
			}
		}
		else
		{
			Seize(pokladna1);
		}
		
		if (Random() > 0.15) // student ma dostatek penez na konte
		{
			Wait(Exponential(7));
		}
		else
		{
			Wait(Uniform(10, 15));
		}

		if(paramPokladny == 2)
		{
			if (pok == 1)
			{
				pokladna1H(Time);
				Release(pokladna1);	
			}
			else
			{
				pokladna2H(Time);
				Release(pokladna2);
			}
		}
		else
		{
			pokladna1H(Time); // ulozeni hodnoty do histogramu
			Release(pokladna1);
		}

		Enter(pocetMist, 1); // zabrani zidle

		Wait(Uniform(9 * MINUTA, 15 * MINUTA)); // obedvani

		Leave(pocetMist, 1); // vraceni zidle a opusteni menzy
	}
}; // konec tridy student

class Prichod : public Event
{
	void Behavior()
	{
		(new Student)->Activate();
		//cout << "TEST" << endl;
		if (paramOptimalizace == 0) // Standartni rozlozeni prichodu studentu
		{
			// V kazdou celou hodinu prijde vice studentu
			if (((Time > (1*HODINA)) && (Time < (1*HODINA+10*MINUTA))) ||
				((Time > (2*HODINA)) && (Time < (2*HODINA+5*MINUTA))) ||
				((Time > 3*HODINA) && (Time < (3*HODINA+5*MINUTA))))
			{
				Activate(Time+Exponential(5));
			}
			// Nejvice lidi prijde na zacatku (pravdepodobne chteji vybirat z hodne jidel)
			else if ((Time > 0) && (Time < 15*MINUTA))
			{
				Activate(Time+Exponential(5));
			}
			// 5 minut pred zaviraci dobou uz nikdo neprijde (vypozorovano pri mereni)
			else if (Time > (3*HODINA+25*MINUTA))
			{
				;
			}
			else 
			{
				Activate(Time+Exponential(15));
			}
		}

		else if (paramOptimalizace == 1) // Trosku lepsi rozlozeni prichodu studentu v case
		{
			if (((Time > 0) && (Time < 10*MINUTA)) ||
				((Time > 30*MINUTA) && (Time < 40*MINUTA)) ||
				((Time > (1*HODINA)) && (Time < (1*HODINA+10*MINUTA))) ||
				((Time > (1*HODINA+30*MINUTA)) && (Time < (1*HODINA+40*MINUTA))) ||
				((Time > (2*HODINA)) && (Time < (2*HODINA+10*MINUTA))) ||
				((Time > (2*HODINA+30*MINUTA)) && (Time < (2*HODINA+40*MINUTA))) ||
				((Time > (3*HODINA)) && (Time < (3*HODINA+10*MINUTA))))
			{
				Activate(Time+Exponential(8));
			}
			// 5 minut pred zaviraci dobou uz nikdo neprijde (vypozorovano pri mereni)
			else if (Time > (3*HODINA+25*MINUTA))
			{
				;
			}
			else
			{
				Activate(Time+Exponential(16));
			}

		}

		else if (paramOptimalizace == 2) // Znacne lepsi rozlozeni prichodu studentu v case
		{
			if (((Time > 0) && (Time < 20*MINUTA)) ||
				((Time > 30*MINUTA) && (Time < 50*MINUTA)) ||
				((Time > (1*HODINA)) && (Time < (1*HODINA+20*MINUTA))) ||
				((Time > (1*HODINA+30*MINUTA)) && (Time < (1*HODINA+50*MINUTA))) ||
				((Time > (2*HODINA)) && (Time < (2*HODINA+20*MINUTA))) ||
				((Time > (2*HODINA+30*MINUTA)) && (Time < (2*HODINA+50*MINUTA))) ||
				((Time > (3*HODINA)) && (Time < (3*HODINA+20*MINUTA))))
			{
				Activate(Time+Exponential(10));
			}
			// 5 minut pred zaviraci dobou uz nikdo neprijde (vypozorovano pri mereni)
			else if (Time > (3*HODINA+25*MINUTA))
			{
				;
			}
			else
			{
				Activate(Time+Exponential(22));
			}
		}

		else if (paramOptimalizace == 3) // Idealni rozlozeni studentu v case
		{
			Activate(Time+Exponential(12));
			if (Time > (3*HODINA+25*MINUTA))
			{
				;
			}
		}

		else if (paramOptimalizace == 4) // vygenerovani 150 lidi v jeden casovy okamzik (simalace otevreni menzy kdy prijde nejvice lidi) 
		{

			if (pocetS < 100)
			{
				Activate(Time+Exponential(1));
				pocetS++;
			}
		}
	}
	
};

int main(int argc, char **argv)
{
	// primitivni zpracovani argumentu pro "make run"
	if (argc >= 2)
	{
		if (argv[1][0] == '2')	// prvni argument udava pocet pokladen (1-2)
		{
			paramPokladny = 2;
		}
	}
	if (argc >= 3)
	{
		if (argv[2][0] == '2')	// druhy argument udava pocet pultu pro vydej hlavnich jidel (1-2)
		{
			paramPultHlavniJidlo = 2;
		}
	}
	if (argc >= 4)
	{
			pripravaPrav = strtod(argv[3], NULL);	// pravdepodobnost ze kucharka bude mit nachystane jidlo, ktere pozadujeme
			if (pripravaPrav == 1.00)
			{
				std::cerr << "Kucharka neni telepat!" << endl;
				exit(1);
			}
			else if ((pripravaPrav > 1.00) || (pripravaPrav < 0.00))
			{
				std::cerr << "Zopakujte si pravdepodobnost!" << endl;
				exit(1);
			}
	}

	if (argc >=5)	// optimalizace prichodu studentu do menzy (0-3)
	{
		if (argv[4][0] == '1')
		{
			paramOptimalizace = 1;
		}
		else if (argv[4][0] == '2')
		{
			paramOptimalizace = 2;
		}
		else if (argv[4][0] == '3')
		{
			paramOptimalizace = 3;
		}
		else if (argv[4][0] == '4')
		{
			paramOptimalizace = 4;
		}
	}
	if (argc >= 6)		// jmeno souboru do ktereho bude tisknut vystup simulace(defaultne "Menza.out")
	{
		filename = argv[5];
		filename.append(".out");	
	}
	// konec zpracovani argumentu


	Init(0, PROVOZNIDOBA);
	(new Prichod)->Activate();
	RandomSeed(time(NULL));
	SetOutput(filename.c_str());
	Run();

// vypisy jednotlivych casti modelu
	fac1.Output();
	fac2.Output();
	fac3.Output();
	if (paramPultHlavniJidlo == 2)
	{
		fac4.Output();
	}
	pokladna1.Output();
	if(paramPokladny == 2)
	{
		pokladna2.Output();
	}
	pocetMist.Output();

// vypis histogramu
	pokladna1H.Output();
	obed1H.Output();
	obed2H1.Output();
	polevkaH.Output();
	prichodH.Output();
	if (paramPultHlavniJidlo == 2)
	{
		obed2H2.Output();
	}
	if(paramPokladny == 2)
	{
		pokladna2H.Output();
	}
}