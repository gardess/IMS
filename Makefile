#Makefile pro IMS projekt
#Autor: Milan Gardáš <xgarda04@stud.fit.vutbr.cz>

CXX=g++
CXXFLAGS= -std=c++11 -Wall -Wextra -pedantic -g 
LDFLAGS = -lsimlib
PROJ=proj

all: build

build: $(PROJ).cpp
	$(CXX) $(CXXFLAGS) $(PROJ).cpp -o $(PROJ) $(LDFLAGS)

clean:
	rm -f $(PROJ) xgarda04.tar 

tar: *.cpp *.h Makefile
	tar -cvf xgarda04.tar *.cpp *.h Makefile report.pdf

commit:
	git commit -a
	git pull origin master
	git push origin master