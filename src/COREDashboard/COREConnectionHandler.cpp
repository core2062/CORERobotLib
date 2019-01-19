#include "COREConnectionHandler.h"

using namespace CORE;
using namespace seasocks;

void COREConnectionHandler::onConnect(WebSocket *connection) {
    m_connections.insert(connection);
    CORELog::LogInfo("Adding Connection");
}

void COREConnectionHandler::onData(WebSocket *webSocket, const char *data) {
    if(std::string(data) == "debug") {
        webSocket->send("This is debug data!");
    } else if(std::string(data) == "driver") {
        webSocket->send("This is driver data!");
    }
}

void COREConnectionHandler::onData(WebSocket *webSocket, const uint8_t *uint8, size_t size) {

}

void COREConnectionHandler::onDisconnect(WebSocket *connection) {
    m_connections.erase(connection);
}