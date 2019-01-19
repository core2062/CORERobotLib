#pragma once

#include <string>
#include "COREFramework/CORETask.h"
#include "CORELogging/CORELog.h"
#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#endif

using namespace std;

namespace CORE {
    class CORENamedObject: public CORETask {
    public:
        CORENamedObject();
        string GetName();
        void RobotInitTask();
    private:
        string m_name;
    };
}