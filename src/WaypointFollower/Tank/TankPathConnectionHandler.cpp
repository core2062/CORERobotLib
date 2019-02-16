#include "TankPathConnectionHandler.h"

using namespace CORE;
using namespace nlohmann;

//set<WebSocket *> COREPathConnectionHandler::m_connections;
//CORETimer * COREPathConnectionHandler::timer = new CORETimer();

TankPath TankPathConnectionHandler::path;

TankPathConnectionHandler::TankPathConnectionHandler() {
//    timer->Start();
}

void TankPathConnectionHandler::onConnect(WebSocket *connection) {
//    m_connections.insert(connection);
    //connection->send(COREDataManager::getJSON(false).dump());
}

void TankPathConnectionHandler::onData(WebSocket *webSocket, const char *string) {
    path = TankPath::FromText(string, false);
    //TODO: Better place to put path
}

void TankPathConnectionHandler::onDisconnect(WebSocket *connection) {
//    m_connections.erase(connection);
}

void TankPathConnectionHandler::Send() {
//    if(timer->Get() > updateRate) {
//        timer->Reset();
//        timer->Start();
//        for (auto connection : m_connections) {
//            connection->send(COREDataManager::getJSON());
//        }
//    }
}

