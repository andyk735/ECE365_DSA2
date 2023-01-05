useHeap.exe: useHeap.o heap.o hash.o
	g++ -o useHeap.exe useHeap.o heap.o hash.o -std=c++11

useHeap.o: useHeap.cpp
	g++ -c useHeap.cpp -std=c++11

heap.o: heap.cpp heap.h
	g++ -c heap.cpp -std=c++11

hash.o: hash.cpp hash.h
	g++ -c hash.cpp -std=c++11

debug:
	g++ -g -o useHeapDebug.exe useHeap.cpp heap.cpp hash.cpp -std=c++11

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backupsa
	cp Makefile backups
