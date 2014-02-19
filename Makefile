model = 20Newsgroups/model

all : nativeBayes train.mtx

nativeBayes : nativeBayes.cpp train.mtx
	cnyCC -D_GLIBCXX -o nativeBayes nativeBayes.cpp ${HOME}/misc/r3.o ${HOME}/misc/packetEncoder.o ${HOME}/misc/cpSMVM.s ${HOME}/misc/mmio.o
	nativeBayes

train.mtx : bowToMtxPy $(model)
	bowToMtxPy
	echo "train.mtx:"
	head train.mtx

small :
	bowToMtxPy smallSet
	echo "train.mtx:"
	head train.mtx
	g++ -D_GLIBCXX -o nativeBayes nativeBayes.cpp
	nativeBayes

clean :
	rm -rf *.mtx 20Newsgroups/model log *.cls

cleanAll :
	rm -rf 20Newsgroups *.mtx

20Newsgroups :
	run20Newsgroups small

$(model) : 
	run20Newsgroups small
