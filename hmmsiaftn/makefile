all :

one :
	getData

rainbowQuery :
	time rainbow -d ./model --method=knn --knn-k=30 --knn-weighting=nnn.nnn --query=bowFiles/00/0000000

two : train.mtx query
	query

three : train.mtx queryParallel
	queryParallel

fourPartOne : queryCny

fourPartTwo : runQueryCny

train.mtx : bowToMtxPy $(model)
	bowToMtxPy
	echo "train.mtx:"
	head train.mtx

mal.o : mal.cpp
	cnyCC -I${HOME}/include -c mal.cpp

query : query.cpp ../spmvCoreNaive.o
	g++ -std=c++0x -I../. -O3 -o query query.cpp ../spmvCoreNaive.o

queryParallel : query.cpp ../spmvCoreParallel.o
	g++ -I../. -pthread -O3 -o queryParallel query.cpp ../spmvCoreParallel.o

queryCny : query.cpp ../spmvCoreCny.o
	cnyCC -O3 -lrt -I../. -L${HOME}/lib -lboost_chrono -lboost_system -o queryCny query.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/mmio.o ${HOME}/misc/cpSMVM.s ../spmvCoreCny.o

runQueryCny :
	scp linux-5.ece.iastate.edu:~/kNN/mal/hmmsiaftn/queryCny .
	queryCny

clean :
	rm -rf queryCny
