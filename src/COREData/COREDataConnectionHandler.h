#pragma once

#include <set>
#include "COREData.h"
#include "COREFramework/COREScheduler.h"
#include "json.hpp"
#include "seasocks/Server.h"

//using namespace std;
using namespace seasocks;

namespace CORE {
    class COREDataConnectionHandler : public WebSocket::Handler {
    public:
        COREDataConnectionHandler();
        void onConnect(WebSocket *connection) override;
        void onData(WebSocket *webSocket, const char *string) override;
        void onDisconnect(WebSocket *connection) override;
        static void send();
    private:
        static set<WebSocket *> m_connections;
        static constexpr double updateRate = 0.1;
        static CORETimer * timer;
    };
}
