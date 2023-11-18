

#ifndef MenuStrategy_h
#define MenuStrategy_h

#include <sstream>
#include <string>
#include <sys/socket.h>
#include <cstdio>
#include <string>

#include "../ClientModel.h"
#include "../../utils.h"
#include "../KNN/KNN.hpp"
#include "../ClientModel.h"
#include "../KNN/KNN.hpp"
#include "../../IO.h"
#include "../KNN/Distance.hpp"

class MenuStrategy
{
public:
    virtual ~MenuStrategy() = default;
    virtual void perform(int client_sock, ClientModel* model, DefaultIO* io) = 0;
    static MenuStrategy* run(char option);

};

class InitializeStrategy : public MenuStrategy {
public:
    // in a real environment one should pass Capabilities array and send their description...
    void perform(int client_sock, ClientModel* data,DefaultIO* io)  override {
        std::string message = R"(Welcome to the KNN Classifier Server. Please choose an option:
1. upload an unclassified csv data file
2. algorithm settings
3. classify data
4. display results
5. download results)";

        ::send(client_sock, message.c_str(), message.length(), 0);

    }
};

class OptionOneStrategy : public MenuStrategy {

private:
    static void parse(const std::string& message, ClientModel* client, bool is_train) {
        std::string cell;

        std::stringstream lineStream(message);
        Flower d;
        while (std::getline(lineStream, cell, ',')) {
            try {
                const std::string& num = (cell);
                d.vector.push_back(stod(num));
            }
            catch (std::invalid_argument&) {
                d.str = cell;
            }
        }
        if (is_train) {
            client -> trainFlower.push_back(d);
        }else {
            client -> testFlower.push_back(d);
        }
    }

public:
    // in a real environment one should pass Capabilities array and send their description...
    void perform(int client_sock, ClientModel* client, DefaultIO* io)  override {
        client -> trainFlower.clear();
        client -> testFlower.clear();
        client -> k_closets.clear();

        std::string message = io->read();
        if (message.at(0) == '0') {
            client -> trainFlower.clear();
            client -> testFlower.clear();
            return;
        }

        while (!message.empty() && !std::equal(message.begin(), message.end(), "1")) {
            parse(message, client, true);
            ::send(client_sock, "1", 1, 0);
            message = io->read();
        }

        std::string m = "Upload Complete";
        m.push_back('\0');
        ::send(client_sock, m.c_str(), m.length(), 0);
        message.clear();

        message = io->read();
        if (message.at(0) == '0') {
            client -> trainFlower.clear();
            client -> testFlower.clear();
            return;
        }

        while (!message.empty() && !std::equal(message.begin(), message.end(), "1")) {
            parse(message, client, false);
            ::send(client_sock, "1", 1, 0);
            message = io->read();
        }

        ::send(client_sock, m.c_str(), m.length(), 0);
    }
};


class OptionTwoStrategy : public MenuStrategy {
public:

    void perform(int client_sock, ClientModel* client, DefaultIO* io)  override {
        std::string message = io->read();
        if (!message.empty() && !std::equal(message.begin(), message.end(), "0") ) {
            std::vector<std::string> m = utils::split<std::string>(message, ' ', nullptr);
            client -> k = std::stoi(m.at(0));
            client -> type = Distance::map(m.at(1));
        }

        std::stringstream ss;
        ss << "The current KNN parameters are: K = " << client -> k << ",  distance metric = " << Distance::map(client -> type);

        ::send(client_sock, ss.str().c_str(), ss.str().length(), 0);

    }
};


class OptionThreeStrategy : public MenuStrategy {
public:
    void perform(int client_sock, ClientModel* client, DefaultIO* io)  override {
        std::string m = "please upload data";
        if (client -> testFlower.empty()) {
            m.push_back('\0');
            ::send(client_sock, m.c_str(), m.length(), 0);
            return;
        }

        KNN knn;
        client -> k_closets = knn.start_knn(client);
        m.clear();
        m = "classifying data complete";
        m.push_back('\0');
        ::send(client_sock, m.c_str(), m.length(), 0);
    }
};


class OptionFourStrategy : public MenuStrategy {
public:
    void perform(int client_sock, ClientModel* client, DefaultIO* io)  override {
        if (client -> trainFlower.empty()) {
            std::string m = "please upload data";
            m.push_back('\0');
            ::send(client_sock, m.c_str(), m.length(), 0);
            return;
        }

        if (client -> k_closets.empty()) {
            std::string m = "please classify data";
            m.push_back('\0');
            ::send(client_sock, m.c_str(), m.length(), 0);
            return;
        }

        std::string m = "1";
        m.push_back('\0');
        ::send(client_sock, m.c_str(), m.length(), 0);

        std::string f = io->read();

        if(!std::equal(f.begin(),f.end(), "1")) {
            return;
        }

        for (int i = 0 ; i < client->k_closets.size() ; i++) {
            std::stringstream ss;
            ss << i+1 << " " << client->k_closets.at(i);
            ::send(client_sock, ss.str().c_str(), ss.str().length(), 0);
            std::string flag = io->read();
            if(!std::equal(flag.begin(),flag.end(), "1")) {
                ::perror("something went wrong");
                return;
            }
        }
        std::string done = "0";
        done.push_back('\0');
        ::send(client_sock, done.c_str(), done.length(), 0);
    }
};


MenuStrategy* MenuStrategy::run(char option) {
    if(option == '0') {
        return new InitializeStrategy();
    }else if (option == '1') {
        return new OptionOneStrategy();
    }else if (option == '2') {
        return new OptionTwoStrategy();
    }else if (option == '3') {
        return new OptionThreeStrategy();
    }else if (option == '4') {
        return new OptionFourStrategy();
    }else if (option == '5') {
        return new OptionFourStrategy();
    }
    return nullptr;
}


#endif /* MenuStrategy_h */
