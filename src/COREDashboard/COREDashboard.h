#pragma once

#include "CORELogging/CORELog.h"
#include "COREConnectionHandler.h"
#include "seasocks/Server.h"
#include "seasocks/PrintfLogger.h"
#include "seasocks/Logger.h"
#include <memory>
#include <thread>
#include <string>

using namespace seasocks;
using namespace std;

namespace CORE {
    class COREDashboard {
    private:
        class m_logger : public seasocks::Logger {
        public:
            void log(Level level, const char *message) override;
        };
        shared_ptr<thread> m_serveThread;
        Server m_server;
    public:
        COREDashboard();
        ~COREDashboard();
    };
}