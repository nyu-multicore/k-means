GCC_BIN=gcc
GCC_STD=c99

.PHONY: clean

all: kmeans

kmeans: src/kmeans.c
	mkdir -p out
	$(GCC_BIN) -g -fopenmp -Wall -o out/kmeans -std=$(GCC_STD) src/kmeans.c -lm

clean:
	rm -rf out
