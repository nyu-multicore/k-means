GPP_BIN=g++
GPP_STD=c++17

.PHONY: clean

all: kmeans_seq kmeans

kmeans_seq: src/kmeans_seq.cpp
	mkdir -p out
	$(GPP_BIN) -g -fopenmp -Wall -o out/kmeans_seq -std=$(GPP_STD) src/utils/* src/kmeans_seq.cpp -lm

kmeans: src/kmeans.cpp
	mkdir -p out
	$(GPP_BIN) -g -fopenmp -Wall -o out/kmeans -std=$(GPP_STD) src/utils/* src/kmeans.cpp -lm

clean:
	rm -rf out
