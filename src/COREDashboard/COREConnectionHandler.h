#pragma once

#include "seasocks/Server.h"
#include "CORELogging/CORELog.h"
#include <string>

using namespace seasocks;
using namespace std;

namespace CORE {
    class COREConnectionHandler : public WebSocket::Handler {
    private:
        set<WebSocket*> m_connections;
    public:
        void onConnect(WebSocket *connection) override;
        void onData(WebSocket *webSocket, const char *data) override;
        void onData(WebSocket *webSocket, const uint8_t *uint8, size_t size) override;
        void onDisconnect(WebSocket *connection) override;
    };
}