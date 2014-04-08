all : query queryParallel queryCny

parsePy :
	parsePy example2.xml

classifierService : classifierService.cpp ../mal.o
	cnyCC -O3 -I${HOME}/include -lrt -L${HOME}/lib -ltardis ${HOME}/tardis/tardis.o -o classifierService classifierService.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o ../mal.o 
	#g++ -o classifierService classifierService.cpp mal.o
	classifierService

mal.o : mal.cpp
	cnyCC -I${HOME}/include -c mal.cpp

query : query.cpp ../spmvCoreNaive.o
	g++ -I../. -O3 -o query query.cpp ../spmvCoreNaive.o
	query

queryParallel : query.cpp ../spmvCoreParallel.o
	g++ -I../. -pthread -O3 -o queryParallel query.cpp ../spmvCoreParallel.o
	queryParallel

queryCny : query.cpp ../spmvCoreCny.o
	cnyCC -O3 -lrt -I../. -L${HOME}/lib -lboost_chrono -lboost_system -o queryCny query.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/mmio.o ${HOME}/misc/cpSMVM.s ../spmvCoreCny.o

runQueryCny :
	scp linux-5.ece.iastate.edu:~/kNN/mal/hmmsiaftn/queryCny .
	queryCny

clean :
	rm -rf data
