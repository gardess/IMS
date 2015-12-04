#Makefile pro IMS projekt
#Autor: Milan Gardáš <xgarda04@stud.fit.vutbr.cz>

CXX=g++
CXXFLAGS= -std=c++11 -Wall -Wextra -pedantic -g 
LDFLAGS = -lsimlib
PROJ=proj

all: build

build: $(PROJ).cpp
	$(CXX) $(CXXFLAGS) $(PROJ).cpp -o $(PROJ) $(LDFLAGS)

run:
	clear
	./$(PROJ)

clean:
	rm -f $(PROJ) 07_xgarda04_xgergu01.zip 

zip: *.cpp *.h Makefile
	zip 07_xgarda04_xgergu01.zip *.cpp *.h Makefile dokumentace.pdf

commit:
	git commit -a
	git pull origin master
	git push origin master