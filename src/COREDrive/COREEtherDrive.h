#pragma once

#include <iostream>

#include <COREDrive.h>
#include <CORETask.h>

namespace CORE {
    class COREEtherDrive : public COREDrive {
    protected:
        void update() override;
    };
}