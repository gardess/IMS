#Makefile pro IMS projekt
#Autor: Milan Gardáš <xgarda04@stud.fit.vutbr.cz>

# /.proj (pocet pokladen(1-2)) (pocet pultu pro vydej hlavnich jidel(1-2)) (procento ve kterem budou mit kucharky pripravene jidlo(0.00-0.99)) (cislo optimalizace prichodu (0-3)) (vystupni soubor)

CXX=g++
CXXFLAGS= -std=c++11 -Wall -Wextra -pedantic -g 
LDFLAGS = -lsimlib
PROJ=proj

all: build

build: $(PROJ).cpp
	$(CXX) $(CXXFLAGS) $(PROJ).cpp -o $(PROJ) $(LDFLAGS)

run:
	./$(PROJ) 1 1 0.25 0 experiment1 # 1 pokladna, 1 pult pro hlavni jidla
	./$(PROJ) 2 1 0.25 0 experiment2 # 2 pokladny
	./$(PROJ) 1 2 0.25 0 experiment3 # 2 pulty
	./$(PROJ) 2 2 0.25 0 experiment4 # 2 pokladny, 2 pulty
	./$(PROJ) 1 1 0.75 0 experiment5 # zvysena pravdepodobnost pripraveneho jidla na pultu
	./$(PROJ) 1 1 0.25 3 experiment6 # idealni prichod studentu (1 a 2 v parametru zatim nejsou naprogramovane)

clean:
	rm -f $(PROJ) 07_xgarda04_xgergu01.zip 

zip:
	zip 07_xgarda04_xgergu01.zip *.cpp Makefile dokumentace.pdf

commit:
	git commit -a
	git pull origin master
	git push origin master