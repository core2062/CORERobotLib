#pragma once

#include "CORELogging/CORELog.h"
#include "COREConnectionHandler.h"
#include "COREData/COREDataConnectionHandler.h"
#include "COREFramework/CORETask.h"
#include "seasocks/Server.h"
#include "seasocks/PrintfLogger.h"
#include "seasocks/Logger.h"
#include "WaypointFollower/COREPathConnectionHandler.h"
#include <memory>
#include <thread>
#include <string>

using namespace seasocks;
//using namespace std;

namespace CORE {
    class COREDashboard : public CORETask {
    private:
        class m_logger : public seasocks::Logger {
        public:
            void log(Level level, const char *message) override;
        };
        static std::shared_ptr<thread> m_serveThread;
        static Server * m_server;
    public:
        static void robotInit();
        void PostLoopTask() override;
        ~COREDashboard();
    };
}