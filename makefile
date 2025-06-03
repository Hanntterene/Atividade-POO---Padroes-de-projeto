programa: hoteis.cpp estrategiadesconto.cpp
	g++ hoteis.cpp estrategiadesconto.cpp -o programa

run: programa
	./programa
clean:
	rm -f programa
