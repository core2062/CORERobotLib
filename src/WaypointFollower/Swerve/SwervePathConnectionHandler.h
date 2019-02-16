#pragma once

#include <set>
#include "COREFramework/COREScheduler.h"
#include "json.hpp"
#include "seasocks/Server.h"
#include "SwerveWaypointFollower.h"

//using namespace std;
using namespace seasocks;

namespace CORE {
    class SwervePathConnectionHandler : public WebSocket::Handler {
    public:
        SwervePathConnectionHandler();
        void onConnect(WebSocket *connection) override;
        void onData(WebSocket *webSocket, const char *string) override;
        void onDisconnect(WebSocket *connection) override;
        static void Send();
        static SwervePath path;
    private:
//        static set<WebSocket *> m_connections;
        static constexpr double updateRate = 0.1;
//        static CORETimer * timer;
    };
}