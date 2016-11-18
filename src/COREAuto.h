#pragma once

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include "COREHardware/CORETimer.h"
#include "CORERobot.h"

#include "WPILib.h"

using namespace std;
namespace CORE {

    enum actionStatus {
        CONTINUE,
        BACKGROUND,
        END
    };

    class COREAutoAction {
    public:
        virtual void actionInit() {}

        virtual void actionEnd() {}

        virtual actionStatus action() = 0;

        virtual ~COREAutoAction() {}
    };

    class Node {
    public:
        Node(COREAutoAction *action);
        void addNext(Node *childNode);
        void addAction(COREAutoAction *leaf);
        void addCondition(bool(*startCondition)());
        bool start(bool lastNodeComplete);
        void doActions();
    private:
        vector<Node *> children;
        vector<COREAutoAction *> actions;
        bool m_startConditonGiven = false;
        bool (*m_startCondition)();
    };

    class COREAuto {
    public:
        COREAuto();
        void auton();
    protected:
        void addFirstNode(Node *firstNode);
        virtual void addNodes()=0;
    private:
        vector<Node *> m_firstNode;
    };
}