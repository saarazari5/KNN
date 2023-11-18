
#ifndef Context_h
#define Context_h

#include "MenuStrategy.h"
class Context {
public:
    Context() = default;

    void set_strategy(MenuStrategy* strategy) {
        this -> strategy_ = strategy;
    }
    
     void execute(int client_sock, ClientModel *model,DefaultIO* io) {
        return strategy_->perform(client_sock, model, io);
    }

    ~Context() {
        delete strategy_;
    }

private:
    MenuStrategy* strategy_{};
};

#endif 
