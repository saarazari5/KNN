all: client.out server.out

client.out: Client/Client.cpp utils.h
	g++ -std=c++11 Client/Client.cpp -pthread  -o client.out


server.out: Server/Strategy/MenuStrategy.h Server/Server.cpp  CLI.h Server/ClientModel.h Server/Strategy/Context.h Server/KNN/Flower.h utils.h Server/KNN/KNN.hpp IO.h Server/KNN/Distance.hpp Server/Server.h

	g++ -std=c++11  Server/Server.cpp -pthread  -o server.out
