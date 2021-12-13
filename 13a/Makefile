.PHONY: run clean

main.out: main.cpp
	$(CXX) -std=c++20 -Wall -Werror -pedantic -O3 main.cpp -o main.out

run: main.out
	./main.out

clean:
	rm *.out