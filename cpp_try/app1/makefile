app1 : main.o ModuleA.o
	g++ -o app1 main.o ModuleA.o

main.o : main.cpp Module/ModuleA.h
	g++ -c main.cpp

ModuleA.o : Module/ModuleA.cpp Module/ModuleA.h
	g++ -c Module/ModuleA.cpp Module/ModuleA.h

clean:
	rm app1 main.o ModuleA.o

