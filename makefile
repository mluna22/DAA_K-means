CC=g++
OUT=kmeans
SRC=src/
INCLUDE=include/

main: $(SRC) $(INCLUDE)*.h
	$(CC) -std=c++17 -o $(OUT) $(SRC)* -I$(INCLUDE)

.PHONY: clean
clean:
	rm -rf *.o
