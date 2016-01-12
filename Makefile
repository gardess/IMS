#Makefile pro IMS projekt
#Autori: Milan Gardáš <xgarda04@stud.fit.vutbr.cz>
#		 Denis Gerguri <xgergu01@stud.fit.vutbr.cz>

# /.proj (pocet pokladen(1-2)) (pocet pultu pro vydej hlavnich jidel(1-2)) (procento ve kterem budou mit kucharky pripravene jidlo(0.00-0.99)) (cislo optimalizace prichodu (0-3)) (vystupni soubor)

CXX=g++
CXXFLAGS= -std=c++11 -Wall -Wextra -pedantic -g 
LDFLAGS = -lsimlib
PROJ=proj

all: build

build: $(PROJ).cpp
	$(CXX) $(CXXFLAGS) $(PROJ).cpp -o $(PROJ) $(LDFLAGS)

run:
	./$(PROJ) 2 1 0.25 0 experiment_1 # 2 pokladny
	./$(PROJ) 1 2 0.25 0 experiment_2 # 2 pulty
	./$(PROJ) 2 2 0.25 0 experiment_3 # 2 pokladny, 2 pulty
	./$(PROJ) 1 1 0.25 4 experiment_4 # otevření menzy - v malém časovém okamžiku příjde 150 lidí
	./$(PROJ) 1 1 0.25 0 experiment_5_1 # standardni prichod studentu do menzy
	./$(PROJ) 1 1 0.25 1 experiment_5_2 # mirne optimalizovany prichod studentu do menzy
	./$(PROJ) 1 1 0.25 2 experiment_5_3 # zlepseny prichod studentu do menzy
	./$(PROJ) 1 1 0.25 3 experiment_5_4 # idealni prichod zakazniku (studentu)

clean:
	rm -f $(PROJ) 07_xgarda04_xgergu01.zip experiment_1.out experiment_2.out experiment_3.out experiment_4.out experiment_5_1.out experiment_5_2.out experiment_5_3.out experiment_5_4.out

zip:
	zip 07_xgarda04_xgergu01.zip *.cpp Makefile dokumentace.pdf

commit:
	git commit -a
	git pull origin master
	git push origin master