#include "SwervePathConnectionHandler.h"

using namespace CORE;
using namespace nlohmann;

//set<WebSocket *> COREPathConnectionHandler::m_connections;
//CORETimer * COREPathConnectionHandler::timer = new CORETimer();

SwervePath SwervePathConnectionHandler::path;

SwervePathConnectionHandler::SwervePathConnectionHandler() {
//    timer->Start();
}

void SwervePathConnectionHandler::onConnect(WebSocket *connection) {
//    m_connections.insert(connection);
    //connection->send(COREDataManager::getJSON(false).dump());
}

void SwervePathConnectionHandler::onData(WebSocket *webSocket, const char *string) {
    path = SwervePath::FromText(string, false);
    //TODO: Better place to put path
}

void SwervePathConnectionHandler::onDisconnect(WebSocket *connection) {
//    m_connections.erase(connection);
}

void SwervePathConnectionHandler::Send() {
//    if(timer->Get() > updateRate) {
//        timer->Reset();
//        timer->Start();
//        for (auto connection : m_connections) {
//            connection->send(COREDataManager::getJSON());
//        }
//    }
}

