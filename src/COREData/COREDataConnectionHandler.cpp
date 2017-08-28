#include "COREDataConnectionHandler.h"

using namespace CORE;

set<WebSocket *> COREDataConnectionHandler::m_connections;
CORETimer * COREDataConnectionHandler::timer = new CORETimer();

COREDataConnectionHandler::COREDataConnectionHandler() {
    timer->Start();
}

void COREDataConnectionHandler::onConnect(WebSocket *connection) {
    m_connections.insert(connection);
    //connection->send(COREDataManager::getJSON(false).dump());
}

void COREDataConnectionHandler::onData(WebSocket *webSocket, const char *string) {
    json jsonData;
    try {
        jsonData = json::parse(string);
    } catch (...) {
        CORELog::logError("Error in parsing return packet from driver station!");
    }
    COREDataManager::updateData(jsonData);
}

void COREDataConnectionHandler::onDisconnect(WebSocket *connection) {
    m_connections.erase(connection);
}

void COREDataConnectionHandler::send() {
    if(timer->Get() > updateRate) {
        timer->Reset();
        timer->Start();
        for (auto connection : m_connections) {
            connection->send(COREDataManager::getJSON());
        }
    }
}

