#pragma once

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include <iostream>

#include "WPILib.h"
#include "COREHardware/CORETimer.h"
#include "COREScheduler.h"
#include "CORELog.h"

using namespace std;
namespace CORE {

    enum actionStatus {
        CONTINUE,
        BACKGROUND,
        END
    };

    class COREAutonAction {
    public:
        virtual void actionInit() {}
        virtual void actionEnd() {}
        virtual actionStatus action() = 0;
        virtual ~COREAutonAction() {}
    };

    class Node {
    public:
        Node(COREAutonAction *action1, COREAutonAction *action2 = nullptr, COREAutonAction *action3 = nullptr);
        Node(shared_ptr<COREAutonAction> action1, shared_ptr<COREAutonAction> action2 = nullptr, shared_ptr<COREAutonAction> action3 = nullptr);
        void addNext(Node *childNode);
        void addNext(shared_ptr<Node> childNode);
        void addAction(COREAutonAction *leaf);
        void addAction(shared_ptr<COREAutonAction> leaf);
        void addCondition(bool(*startCondition)());
        bool complete();
        void act(bool lastNodeDone);
    private:
        vector<shared_ptr<Node>> m_children;
        vector<shared_ptr<COREAutonAction>> m_actions;
        bool m_startConditonGiven = false;
        bool m_actionsInitialized = false;
        bool (*m_startCondition)();
    };

    class COREAuton {
    public:
        COREAuton(string name, Node * firstNode, bool defaultAuton = false);
        void auton();
        void autonInit();
        bool complete();
        void putToDashboard(shared_ptr<SendableChooser<COREAuton*>> chooser);
    protected:
        void addFirstNode(Node * firstNode);
        virtual void addNodes() = 0;
    private:
        string m_name;
        bool m_defaultAuton = false;
        vector<Node*> m_firstNode;
    };
}
