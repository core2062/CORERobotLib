#include "COREDashboard.h"

using namespace CORE;

COREDashboard::COREDashboard() : m_server(make_shared<m_logger>()) {
    m_server.addWebSocketHandler("/data", make_shared<COREConnectionHandler>());
    m_serveThread = make_shared<thread>([&]{m_server.serve("CORE-Web-Interface/www", 5810);});
}

COREDashboard::~COREDashboard() {
    m_server.terminate();
    m_serveThread->join();
}

void COREDashboard::m_logger::log(Logger::Level level, const char *message) {
    if(level == Level::SEVERE || level == Level::ERROR) {
        CORELog::logError("COREDashboard: " + string(message));
    } else if(level == Level::WARNING) {
        CORELog::logWarning("COREDashboard: " + string(message));
    } else if(level == Level::INFO /*|| level == Level::ACCESS || level == Level::DEBUG*/) {
        CORELog::logInfo("COREDashboard: " + string(message));
    }
}
