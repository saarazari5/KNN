#ifndef IO_h
#define IO_h

#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <iostream>

using namespace std;

class DefaultIO {
public:
    DefaultIO() = default;
    virtual std::string read() = 0;
    virtual void write(std::string s) = 0;
};

class SocketIO : public DefaultIO {
public:
    explicit SocketIO(int socket_descriptor) : socket_descriptor(socket_descriptor), DefaultIO(){};
    std::string read() override;
    void write(std::string s) override;

private:
    int socket_descriptor;
};

class StandardIO : public DefaultIO {
public:
    StandardIO():DefaultIO(){};
    std::string read() override;
    void write(std::string s) override;
};


string SocketIO::read() {
    char client_message[4096]={0};
    long bytes = recv(socket_descriptor, client_message, sizeof(client_message), 0);
    if (bytes > 0) {
        string message(client_message);
        return message;
    }
    return "";
}

void SocketIO::write(string s) {
    ::send(socket_descriptor, s.c_str(), s.length(), 0);
}


string StandardIO::read() {
    string s;
    cin >> s;
    return s;
}

void StandardIO::write(std::string s) {
    cout << s << endl;
}

#endif /* IO_h */
