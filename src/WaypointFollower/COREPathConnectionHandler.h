#pragma once

#include <set>
#include "COREFramework/COREScheduler.h"
#include "json.hpp"
#include "seasocks/Server.h"
#include "WaypointFollower/WaypointFollower.h"

using namespace std;
using namespace seasocks;

namespace CORE {
    class COREPathConnectionHandler : public WebSocket::Handler {
    public:
        COREPathConnectionHandler();
        void onConnect(WebSocket *connection) override;
        void onData(WebSocket *webSocket, const char *string) override;
        void onDisconnect(WebSocket *connection) override;
        static void Send();
        static Path path;
    private:
//        static set<WebSocket *> m_connections;
        static constexpr double updateRate = 0.1;
//        static CORETimer * timer;
    };
}