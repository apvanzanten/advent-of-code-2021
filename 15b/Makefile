.PHONY: run clean debug_run

main.out: main.cpp
	$(CXX) -std=c++20 -Wall -Werror -pedantic -O3 main.cpp -o main.out

run: main.out
	./main.out
	
main.debug.out: main.cpp
	$(CXX) -std=c++20 -Wall -Werror -pedantic -Og -g main.cpp -o main.debug.out -fsanitize=address


debug_run: main.debug.out
	./main.debug.out


clean:
	rm *.out