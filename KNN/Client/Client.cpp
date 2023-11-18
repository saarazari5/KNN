#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <algorithm>
#include <sstream>

using namespace std;

static string m;


void send_data(const string& path, const string& data) {
    std::ofstream file(path);
    if(!file) return;
    std::stringstream lineStream(data);
    string line;
    while (std::getline(lineStream, line, '\n')) {
        file << line << '\n';
    }
    file.close();
}

void send_line(int sock, const string& line) {
    ::send(sock, line.c_str(), line.size(), 0);
}

bool line_received(int sock) {
    char flag[1] = {0};
    long read_bytes = recv(sock, flag, 1, 0);
    if (read_bytes <= 0) {
        return false;
    }
    if (flag[0] == '1'){
        return true;
    }
    return false;
}

void option5(int sock) {
    std::string single_test = "temp";
    std::string res;
    char message[100] = {0};

    if (recv(sock, message, sizeof(message), 0) <= 0) {
        ::perror("something went wrong");
        return;
    }

    if (message[0] != '1') {
        cout << message << endl;
        return;
    }

    ::memset(message, 0, sizeof(message));
    ::send(sock, "1", 1, 0);

    while (!single_test.empty() && !equal(single_test.begin(), single_test.end(), "0")) {
        long bytes = recv(sock, message, sizeof(message), 0);
        if (bytes <= 0) {
            ::perror("something went wrong");
            return;
        }

        single_test = string(message);
        if(equal(single_test.begin(), single_test.end(), "0")) {
            continue;
        }

        res += single_test;
        res += "\n";
        string success = "1";
        success.push_back('\0');
        ::send(sock, success.c_str(), success.size(), 0);
        ::memset(message, 0 , sizeof (message));
    }

    cout << "Please upload your result CSV path" << endl;
    string path;
    getline(cin, path);

    std::thread t(send_data, path, res);
    t.detach();
}


void option4(int sock) {
    std::string single_test = "temp";
    char message[100] = {0};
    if (recv(sock, message, sizeof(message), 0) <= 0) {
        ::perror("something went wrong");
        return;
    }

    if (message[0] != '1') {
        cout << message << endl;
        return;
    }

    ::memset(message, 0, sizeof(message));
    ::send(sock, "1", 1, 0);

    while (!single_test.empty() && !equal(single_test.begin(), single_test.end(), "0")) {
        long bytes = recv(sock, message, sizeof(message), 0);
        if (bytes <= 0) {
            ::perror("something went wrong");
            return;
        }

        single_test = string(message);
        if(equal(single_test.begin(), single_test.end(), "0")) {
            continue;
        }
        if (single_test.at(single_test.size() - 1) == '\n' || single_test.at(single_test.size() - 1) == '\r' ) {
            cout << message;
        }else {
            cout << message << endl;

        }

        string success = "1";
        success.push_back('\0');
        ::send(sock, success.c_str(), success.size(), 0);
        ::memset(message, 0 , sizeof (message));
    }
}

void option3(int sock) {
    char message[100] = {0};
    long read_bytes = recv(sock, message, sizeof(message), 0);
    if (read_bytes <= 0) {
        ::perror("something went wrong");
    }
    cout << message << endl;
}

void option2(int sock) {
    std::string params;
    getline(cin, params);
    if (params.empty()) {
        params = "0";
        params.push_back('\0');
    }
    ::send(sock, params.c_str(), params.size(), 0);
    char message[100] = {0};
    long read_bytes = recv(sock, message, sizeof(message), 0);
    if (read_bytes <= 0) {
        ::perror("something went wrong");
    }
    cout << message << endl;
}

void option1(int sock, bool is_train) {

    std::string csv;
    if(is_train) {
        cout << "Please upload your local train CSV file" << endl;
    } else {
        cout << "Please upload your local test CSV file" << endl;
    }

    getline(cin, csv);


    std::ifstream file(csv);
    if(!file) {
        cout << "invalid input" << endl;
        ::send(sock, "0", 1, 0);
        return;
    }

    string line;
    while (std::getline(file, line)) {
        line.push_back('\0');
        send_line(sock, line);
        if(!line_received(sock)) { break; }
    }
    line = "1";
    line.push_back('\0');
    ::send(sock, line.c_str(), line.size(), 0);

    char message[100] = {0};
    long read_bytes = recv(sock, message, sizeof(message), 0);
    if (read_bytes <= 0) {
        ::perror("something went wrong");
    }
    cout << message << endl;
    if(is_train) {
        option1(sock, false);
    }else {
        return;
    }
}

void start_option(int sock, const string& option) {
    if (option.at(0) == '1') {
        option1(sock, true);
    } else if(option.at(0) == '2') {
        option2(sock);
    } else if(option.at(0) == '3') {
        option3(sock);
    } else if(option.at(0) == '4') {
        option4(sock);
    } else if(option.at(0) == '5') {
        option5(sock);
    } else  {
        ::perror("invalid option");
        return;
    }
}

void menu(int sock) {
    if (!m.empty()) {
        cout << m <<endl;
        return;
    }

    long read_bytes = 1;
    while (read_bytes > 0) {
        char bufferRec[4096]={0};
        int expected_data_len = sizeof(bufferRec);
        read_bytes = recv(sock, bufferRec, expected_data_len, 0);
        cout << bufferRec <<endl;
        m.append( string(bufferRec));
        if (bufferRec[4095] == '\0') {
            read_bytes = 0;
        }
    }
}

void send_option(int sock, const string& option) {
    ::send(sock, option.c_str(), option.size(), 0);
}

bool response_for_option(int sock) {
    long read_bytes;
    char bufferRec[1]={0};
    int expected_data_len = sizeof(bufferRec);
    read_bytes = recv(sock, bufferRec, expected_data_len, 0);
    if(read_bytes <= 0) {
        return false;
    }
    if (bufferRec[0] == '1') {
        return true;
    } else {
        return false;
    }
}

int main(int argc, char **argv) {

    if (argc < 3) {
        cout << "arguments are missing" << endl;
        return 0;
    }

    const char *ip_address = argv[1];
    string portStr = argv[2];
    const int port_no = stoi(portStr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
        return 0;
    } else {
        struct sockaddr_in sin{};
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(ip_address);
        sin.sin_port = htons(port_no);
        if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("error connecting to server");
            return 0;
        }
        while (true) {
            menu(sock);
            string option;
            getline(cin, option);
            try { std::stoi(option); } catch (std::invalid_argument&) { continue; }

            send_option(sock, option);
            if(option.at(0) == '8') { break; }
            if (!response_for_option(sock)) { continue; }
            start_option(sock, option);
        }

        close(sock);
        return 0;
    }
}
