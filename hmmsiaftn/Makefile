all : query

parsePy :
	parsePy example2.xml

classifierService : classifierService.cpp ../mal.o
	cnyCC -O3 -I${HOME}/include -lrt -L${HOME}/lib -ltardis ${HOME}/tardis/tardis.o -o classifierService classifierService.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o ../mal.o 
	#g++ -o classifierService classifierService.cpp mal.o
	classifierService

mal.o : mal.cpp
	cnyCC -I${HOME}/include -c mal.cpp

query : query.cpp
	g++ -O3 -o query query.cpp
	query

clean :
	rm -rf data

