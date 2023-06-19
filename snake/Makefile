
play: Stage.o game.o snake.o Main.o
	g++ -g -std=c++11 -o play Stage.o game.o snake.o Main.o -lncursesw

clean:
	rm -f *.o play

%.o : %.cpp %.h
	g++ -g -std=c++11 -c $<

%.o : %.cpp
	g++ -g -std=c++11 -c $<

% : %.cpp
	g++ -g -std=c++11 -o $@ $<