FLAGS = -pedantic-errors -std=c++11

main: main.o action.o board.o cell.o
	g++ $(FLAGS) $^ -o $@

cell.o: cell.cpp cell.h
	g++ $(FLAGS) -c $<

action.o: action.cpp action.h cell.h
	g++ $(FLAGS) -c $<

board.o: board.cpp board.h cell.h
	g++ $(FLAGS) -c $<

main.o: main.cpp board.h action.h
	g++ $(FLAGS) -c $<

clean:
	rm -f main main.o action.o board.o cell.o

.PHONY: clean
