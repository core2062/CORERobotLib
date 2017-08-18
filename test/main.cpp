#include <unistd.h>
#include "COREDashboard/COREDashboard.h"

void Wait(double _) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    Wait(_*1000);
#else
    usleep(_ * 1000000);
#endif
}

int main() {
    CORE::COREDashboard dashboard;
    Wait(100);
}