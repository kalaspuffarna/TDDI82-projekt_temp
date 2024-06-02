a.out: source/main.o source/Entity.o source/State.o source/Game.o source/rcs.o
	g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -fmax-errors=3 source/main.o source/Entity.o source/State.o source/Game.o source/rcs.o -lsfml-graphics -lsfml-window -lsfml-system -o a.out

source/main.o: source/main.cc
	g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -fmax-errors=3 -c source/main.cc -o source/main.o

source/Entity.o: source/Entity.cc
	g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -fmax-errors=3 -c source/Entity.cc -o source/Entity.o

source/State.o: source/State.cc
	g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -fmax-errors=3 -c source/State.cc -o source/State.o

source/Game.o: source/Game.cc
	g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -fmax-errors=3 -c source/Game.cc -o source/Game.o

source/rcs.o: source/rcs.cc
	g++ -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -fmax-errors=3 -c source/rcs.cc -o source/rcs.o

clean: 
	rm source/*.o a.out
