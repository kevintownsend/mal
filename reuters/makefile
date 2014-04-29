all : makeDocuments

makeDocuments : makeDocuments.cpp
	g++ -std=c++0x -o makeDocuments makeDocuments.cpp

one : makeDocuments
	runReuters

two : train.mtx query
	query

three : train.mtx queryParallel
	queryParallel

train.mtx : bowToMtxPy $(model)
	bowToMtxPy
	echo "train.mtx:"
	head train.mtx

mal.o : mal.cpp
	cnyCC -I${HOME}/include -c mal.cpp

query : query.cpp ../spmvCoreNaive.o
	g++ -I../. -O3 -o query query.cpp ../spmvCoreNaive.o

queryParallel : query.cpp ../spmvCoreParallel.o
	g++ -I../. -pthread -O3 -o queryParallel query.cpp ../spmvCoreParallel.o

queryCny : query.cpp ../spmvCoreCny.o
	cnyCC -O3 -lrt -I../. -L${HOME}/lib -lboost_chrono -lboost_system -o queryCny query.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/mmio.o ${HOME}/misc/cpSMVM.s ../spmvCoreCny.o

runQueryCny :
	scp linux-5.ece.iastate.edu:~/kNN/mal/hmmsiaftn/queryCny .
	queryCny
