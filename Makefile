model = 20Newsgroups/model

all : malCny train.mtx spmvCoreCny.o naiveMal spmvCoreMkl.o mklMal spmvCoreParallel.o malParallel

malCny : mal.cpp train.mtx spmvCoreCny.o
	#cnyCC -O3 -DTIME_CP -I${HOME}/include -lrt -L${HOME}/lib -ltardis ${HOME}/tardis/tardis.o -o nativeBayes nativeBayes.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o
	cnyCC -O3 -lrt ${HOME}/tardis/tardis.o -L${HOME}/lib -lboost_chrono -lboost_system -o malCny mal.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o spmvCoreCny.o

naiveMal : mal.cpp spmvCoreNaive.o
	g++ -std=gnu++0x -O3 -lrt -o naiveMal mal.cpp spmvCoreNaive.o ${HOME}/tardis/tardis.o

train.mtx : bowToMtxPy $(model)
	bowToMtxPy
	echo "train.mtx:"
	head train.mtx

small : smallSet/bowMatrix
	bowToMtxPy smallSet
	echo "train.mtx:"
	head train.mtx

smallSet/bowMatrix :
	runSmallSet

clean :
	rm -rf *.mtx 20Newsgroups/model log *.cls

cleanAll :
	rm -rf 20Newsgroups *.mtx

20Newsgroups :
	run20Newsgroups small

$(model) : 
	run20Newsgroups small

convey :
	scp linux-5.ece.iastate.edu:~/kNN/mal/mal .
	mal

spmvCoreCny.o : spmvCoreCny.cpp spmvCore.h
	cnyCC -O3 -lrt -c spmvCoreCny.cpp

spmvCoreNaive.o : spmvCoreNaive.cpp spmvCore.h
	g++ -std=gnu++0x -O3 -c spmvCoreNaive.cpp

spmvCoreParallel.o : spmvCoreNaive.cpp spmvCore.h
	g++ -std=gnu++0x -DPARALLEL -O3 -o spmvCoreParallel.o -c spmvCoreNaive.cpp

malParallel : spmvCoreParallel.o mal.cpp
	g++ -std=gnu++0x -pthread -o malParallel mal.cpp spmvCoreParallel.o

spmvCoreMkl.o : spmvCoreMkl.cpp spmvCore.h
	icpc -std=gnu++0x -mkl=parallel -O3 -c spmvCoreMkl.cpp

mklMal : mal.cpp spmvCoreMkl.o
	icpc -std=gnu++0x -O3 -mkl=parallel -o mklMal mal.cpp spmvCoreMkl.o
