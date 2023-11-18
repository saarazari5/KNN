cmake_minimum_required(VERSION 3.24)
project(knn_clientServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(knn_clientServer Server/Server.cpp CLI.h Server/ClientModel.h Client/Client.cpp Server/Strategy/Context.h Server/Strategy/MenuStrategy.h Server/KNN/Flower.h utils.h Server/Server.h)
