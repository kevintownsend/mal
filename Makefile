model = 20Newsgroups/model

all : mal train.mtx spmvCoreCny.o naiveMal

mal : mal.cpp train.mtx spmvCoreCny.o
	#cnyCC -O3 -DTIME_CP -I${HOME}/include -lrt -L${HOME}/lib -ltardis ${HOME}/tardis/tardis.o -o nativeBayes nativeBayes.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o
	cnyCC -O3 -lrt ${HOME}/tardis/tardis.o -o mal mal.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o spmvCoreCny.o

naiveMal : mal.cpp spmvCoreNaive.o
	g++ -O3 -lrt -o naiveMal mal.cpp spmvCoreNaive.o ${HOME}/tardis/tardis.o

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
	g++ -O3 -c spmvCoreNaive.cpp
