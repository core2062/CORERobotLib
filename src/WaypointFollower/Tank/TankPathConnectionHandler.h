#pragma once

#include <set>
#include "COREFramework/COREScheduler.h"
#include "json.hpp"
#include "seasocks/Server.h"
#include "TankWaypointFollower.h"

//using namespace std;
using namespace seasocks;

namespace CORE {
    class TankPathConnectionHandler : public WebSocket::Handler {
    public:
        TankPathConnectionHandler();
        void onConnect(WebSocket *connection) override;
        void onData(WebSocket *webSocket, const char *string) override;
        void onDisconnect(WebSocket *connection) override;
        static void Send();
        static TankPath path;
    private:
//        static set<WebSocket *> m_connections;
        static constexpr double updateRate = 0.1;
//        static CORETimer * timer;
    };
}