all: echo-server echo-client

echo-server: echo.o echo-server.o
	g++ -o echo-server echo.o echo-server.o -lpthread
	
echo-client: echo.o echo-client.o
	g++ -o echo-client echo.o echo-client.o -lpthread

echo-server.o: echo.h echo-server.cpp

echo-client.o: echo.h echo-client.cpp

echo.o: echo.h echo.cpp 

clean:
	rm -f echo-server echo-client
	rm -f *.o
