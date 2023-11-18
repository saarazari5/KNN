#ifndef CLI_h
#define CLI_h

#include <vector>
#include "Server/Strategy/Context.h"
#include "Server/ClientModel.h"
#include <cstring>

class CLI {
public:
    virtual void start() = 0;
    virtual ~CLI() = default;

};

class Command {
public:
    DefaultIO* io{};
    virtual void execute(CLI* cli) = 0;
    virtual ~Command() = default;

private:
    std::string description;
};

class InitializeCommand : public Command {
public:
    void execute(CLI* cli) override;

private:
    std::string description;
};

class UploadCommand : public Command {

public:
    void execute(CLI* cli) override;

private:
    std::string description = "upload an unclassified csv data file";
};

class SetCommand : public Command {
public:
    void execute(CLI* cli) override;

private:
    std::string description = "algorithm settings";
};


class CalculateCommand : public Command {
public:
    void execute(CLI* cli) override;

private:
    std::string description = "classify data";
};


class PrintResultsCommand : public Command {
public:
    void execute(CLI* cli) override;

private:
    std::string description = "display results";
};


class DownloadResultsCommand : public Command {
public:
    void execute(CLI* cli) override;

private:
    std::string description = "download results";
};



class ServerCLI:CLI {
public:
    ServerCLI(Context* context, ClientModel* clientModel) : context(context),clientModel(clientModel){};
    std::vector<Command*> commands = {new InitializeCommand(),
                                      new UploadCommand(),
                                      new SetCommand(),
                                      new CalculateCommand(),
                                      new PrintResultsCommand(),
                                      new DownloadResultsCommand()};
    Context* context;
    ClientModel* clientModel;
    void start() override;

    ~ServerCLI() override {
        delete context;
        delete clientModel;
    }
};

void ServerCLI::start() {
    commands.at(0)->execute(this);

    while (true) {
        char option[1] = "";
        long read_bytes = recv(clientModel->identifier, option, 1, 0);

        if (read_bytes == 0) {
            printf("connection is closed\n");
            ::send(clientModel -> identifier, "0", 1, 0);
            continue;
        } else if (read_bytes < 0) {
            ::send(clientModel -> identifier, "0", 1, 0);
            perror("something went wrong");
            break;
        }

        if (option[0] == '8') {
            return;
        }

        ::send(clientModel -> identifier, "1", 1, 0);
        commands.at(option[0]- '0')->execute(this);

    }
}


class ClientCLI:CLI {
public:
    void start() override;
};

void ClientCLI::start() {


}

/***
 *
 * server command implementations
 */

void InitializeCommand::execute(CLI* cli) {
    ServerCLI* server = (ServerCLI*) cli;
    this->io = new SocketIO(server -> clientModel->identifier);
    server -> context->set_strategy(MenuStrategy::run('0'));
    server -> context->execute(server -> clientModel -> identifier, server -> clientModel, io);
}

void UploadCommand::execute(CLI* cli) {

    ServerCLI* server = (ServerCLI*) cli;
    this->io = new SocketIO(server -> clientModel->identifier);
    server -> context->set_strategy(MenuStrategy::run('1'));
    server -> context->execute(server -> clientModel -> identifier, server -> clientModel, io);
}

void SetCommand::execute(CLI* cli) {

    ServerCLI* server = (ServerCLI*) cli;
    this->io = new SocketIO(server -> clientModel->identifier);
    server -> context->set_strategy(MenuStrategy::run('2'));
    server -> context->execute(server -> clientModel -> identifier, server -> clientModel, io);
}

void CalculateCommand::execute(CLI* cli) {
    ServerCLI* server = (ServerCLI*) cli;
    this->io = new SocketIO(server -> clientModel->identifier);
    server -> context->set_strategy(MenuStrategy::run('3'));
    server -> context->execute(server -> clientModel -> identifier, server -> clientModel, io);
}

void PrintResultsCommand::execute(CLI* cli) {
    ServerCLI* server = (ServerCLI*) cli;
    this->io = new SocketIO(server -> clientModel->identifier);
    server -> context->set_strategy(MenuStrategy::run('4'));
    server -> context->execute(server -> clientModel -> identifier, server -> clientModel, io);
}

void DownloadResultsCommand::execute(CLI* cli) {
    ServerCLI* server = (ServerCLI*) cli;
    this->io = new SocketIO(server -> clientModel->identifier);
    server -> context->set_strategy(MenuStrategy::run('5'));
    server -> context->execute(server -> clientModel -> identifier, server -> clientModel, io);
}


#endif /* CLI_h */
