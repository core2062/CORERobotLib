#include "COREPathConnectionHandler.h"

using namespace CORE;
using namespace nlohmann;

//set<WebSocket *> COREPathConnectionHandler::m_connections;
//CORETimer * COREPathConnectionHandler::timer = new CORETimer();

Path COREPathConnectionHandler::path;

COREPathConnectionHandler::COREPathConnectionHandler() {
//    timer->Start();
}

void COREPathConnectionHandler::onConnect(WebSocket *connection) {
//    m_connections.insert(connection);
    //connection->send(COREDataManager::getJSON(false).dump());
}

void COREPathConnectionHandler::onData(WebSocket *webSocket, const char *string) {
    path = Path::FromText(string, false);
    //TODO: Better place to put path
}

void COREPathConnectionHandler::onDisconnect(WebSocket *connection) {
//    m_connections.erase(connection);
}

void COREPathConnectionHandler::Send() {
//    if(timer->Get() > updateRate) {
//        timer->Reset();
//        timer->Start();
//        for (auto connection : m_connections) {
//            connection->send(COREDataManager::getJSON());
//        }
//    }
}

