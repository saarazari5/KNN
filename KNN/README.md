# ex4Mitkadem
this is a simple implementation of the KNN algorithm in c++. 
in this project, we succeed to create a client and a server that transfer messages with threads.
the Server side gets his  data storage from that contains information about the type of flower vector.
the client takes from a user a new vector, type of distance, and the numbers of niebgors to search from. the client sends the info to a specific IP address and port number.

input file with the set of vectors.
the distance algorithm one should use to calculate the nearest neighbors.
the program takes another vector from the console and finds what his closest type among the set and prints this type to the screen.

# running the program
to run the program we build "makeFile" with a simple command. so to run just write in the command line make. and then type the command 
server.out file port
-in the server side
* file: is represent the address to take the data from.
* port - is the number of the port to expect an answer.

On the client side:
client.out ip port
* Ip: represent the IP address that the server is on 
* port: the port to send the message from.

both the client and the server should be running on the same address and port number.
# submitted by:
Omer Evan 
Saar Azari
