#pragma once

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include <iostream>
#include "COREHardware/CORETimer.h"
#include "COREScheduler.h"
//#include "CORERobot.h"

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
        Node(COREAutoAction *action1, COREAutoAction *action2 = nullptr, COREAutoAction *action3 = nullptr);
        Node(shared_ptr<COREAutoAction> action1, shared_ptr<COREAutoAction> action2 = nullptr, shared_ptr<COREAutoAction> action3 = nullptr);
        void addNext(Node *childNode);
        void addNext(shared_ptr<Node> childNode);
        void addAction(COREAutoAction *leaf);
        void addAction(shared_ptr<COREAutoAction> leaf);
        void addCondition(bool(*startCondition)());
        bool complete();
        void act(bool lastNodeDone);
    private:
        vector<shared_ptr<Node>> m_children;
        vector<shared_ptr<COREAutoAction>> m_actions;
        bool m_startConditonGiven = false;
        bool m_actionsInitialized = false;
        bool (*m_startCondition)();
    };

    class COREAuto {
    public:
        COREAuto();
        void auton();
        void autonInit();
        void putToDashboard(shared_ptr<SendableChooser> chooser);
        bool complete();
    protected:
        void addFirstNode(Node *firstNode);
        virtual void addNodes() = 0;
        void setName(string name);
        void setDefault(bool defaultAuton);
    private:
        string m_name;
        bool m_defaultAuton = false;
        vector<Node *> m_firstNode;
        shared_ptr<COREAuto> m_instance;
    };
}