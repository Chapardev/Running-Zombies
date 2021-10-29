checks = -Wall -pedantic-errors
libs = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
no_console = -Wl,-subsystem,windows

default:
# windres resources.rc resources.o
	g++ -std=c++17 -O2 -c ../src/*.cpp -I ../include $(checks)
	g++ ./*.o -o app $(libs) $(no_console)
# g++ -std=c++17 -O2 ../src/*.cpp -o app -I ../include $(checks) $(libs) $(no_console)
