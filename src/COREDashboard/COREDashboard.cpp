#include "COREDashboard.h"

using namespace CORE;

std::shared_ptr<thread> COREDashboard::m_serveThread;
Server * COREDashboard::m_server;

void COREDashboard::robotInit() {
    m_server = new Server(std::make_shared<m_logger>());
//    m_server->addWebSocketHandler("/data", make_shared<COREDataConnectionHandler>());
    m_server->addWebSocketHandler("/path", std::make_shared<COREPathConnectionHandler>());

    m_serveThread = std::make_shared<thread>([&]{m_server->serve("/home/lvuser/COREWebDashboard/www", 5810);});
}

COREDashboard::~COREDashboard() {
    m_server->terminate();
    m_serveThread->join();
    delete m_server;
}

void COREDashboard::m_logger::log(Logger::Level level, const char *message) {
    if(level == Level::SEVERE || level == Level::ERROR) {
        CORELog::LogError("COREDashboard: " + std::string(message));
    } else if(level == Level::WARNING) {
        CORELog::LogWarning("COREDashboard: " + std::string(message));
    } else if(level == Level::INFO /*|| level == Level::ACCESS || level == Level::DEBUG*/) {
        CORELog::LogInfo("COREDashboard: " + std::string(message));
    }
}

void COREDashboard::PostLoopTask() {
//    m_server->execute(COREDataConnectionHandler::send);
}