#include "CORESwerveDashboard.h"

using namespace CORE;

std::shared_ptr<thread> CORESwerveDashboard::m_serveThread;
Server * CORESwerveDashboard::m_server;

void CORESwerveDashboard::robotInit() {
    m_server = new Server(std::make_shared<m_logger>());
//    m_server->addWebSocketHandler("/data", make_shared<COREDataConnectionHandler>());
    m_server->addWebSocketHandler("/path", std::make_shared<SwervePathConnectionHandler>());

    m_serveThread = std::make_shared<thread>([&]{m_server->serve("/home/lvuser/COREWebDashboard/www", 5810);});
}

CORESwerveDashboard::~CORESwerveDashboard() {
    m_server->terminate();
    m_serveThread->join();
    delete m_server;
}

void CORESwerveDashboard::m_logger::log(Logger::Level level, const char *message) {
    if(level == Level::Severe || level == Level::Error) {
        CORELog::LogError("COREDashboard: " + std::string(message));
    } else if(level == Level::Warning) {
        CORELog::LogWarning("COREDashboard: " + std::string(message));
    } else if(level == Level::Info /*|| level == Level::ACCESS || level == Level::DEBUG*/) {
        CORELog::LogInfo("COREDashboard: " + std::string(message));
    }
}

void CORESwerveDashboard::PostLoopTask() {
//    m_server->execute(COREDataConnectionHandler::send);
}