#pragma once

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include <iostream>

#include <WPILib.h>
#include "COREHardware/CORETimer.h"
#include "COREScheduler.h"
#include "CORELog.h"

using namespace std;
namespace CORE {
    class COREAutonAction {
    public:
        enum actionStatus {
            CONTINUE,
            BACKGROUND,
            END
        };

        virtual void actionInit() {}
        virtual void actionEnd() {}
        virtual actionStatus action() = 0;
        virtual ~COREAutonAction() {}
    };

    class Node {
    public:
        Node(COREAutonAction *action1, COREAutonAction *action2 = nullptr, COREAutonAction *action3 = nullptr);
        Node(shared_ptr<COREAutonAction> action1, shared_ptr<COREAutonAction> action2 = nullptr,
             shared_ptr<COREAutonAction> action3 = nullptr);
        void addNext(Node *childNode);
        void addNext(shared_ptr<Node> childNode);
        void addAction(COREAutonAction *leaf);
        void addAction(shared_ptr<COREAutonAction> leaf);
        void addCondition(bool(*startCondition)());
        bool complete();
        void reset();
        void act(bool lastNodeDone);
    private:
        vector<shared_ptr<Node>> m_children;
        vector<shared_ptr<COREAutonAction>> m_actions;
        vector<shared_ptr<COREAutonAction>> m_actionsCache;
        bool m_startConditonGiven = false;
        bool m_actionsInitialized = false;
        bool (*m_startCondition)();
    };

    class COREAuton {
    public:
        COREAuton(string name, Node * firstNode, bool defaultAuton = false);
        void putToDashboard(shared_ptr<SendableChooser<COREAuton*>> chooser);
        inline COREAuton* getInstance() {
        	return this;
        }
        inline bool getDefault() {
        	return m_defaultAuton;
        }
        string getName();
        void auton();
        void autonInit();
        bool complete();
        void reset();
    protected:
        void addFirstNode(Node * firstNode);
        virtual void addNodes() = 0;
    private:
        string m_name;
        bool m_defaultAuton = false;
        bool m_nodesAdded = false;
        vector<Node*> m_firstNode;
    };
}
