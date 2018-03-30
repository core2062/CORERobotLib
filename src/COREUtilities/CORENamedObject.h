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
        string getName();
        void robotInitTask();
    private:
        string m_name;
    };
}