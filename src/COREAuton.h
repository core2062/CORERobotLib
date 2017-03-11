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
            END
        };

        virtual void actionInit() {}

        virtual void actionEnd() {}

        virtual actionStatus action() = 0;

        virtual ~COREAutonAction() {}
    };

    class Node {
    public:
        Node(double timeout, COREAutonAction* action1, COREAutonAction* action2 = nullptr, COREAutonAction* action3 = nullptr);
        void addNext(Node* childNode);
        void addAction(COREAutonAction* leaf);
        void addCondition(bool(* startCondition)());
        bool complete();
        void reset();
        void setTimeout(double timeout);
        void act(bool lastNodeDone);
        ~Node();
    private:
        vector<Node*> m_children;
        vector<COREAutonAction*> m_actions;
        bool m_startConditonGiven = false;
        bool m_actionsInitialized = false;
        bool (* m_startCondition)();
        double m_timeout;
        CORETimer m_timer;
    };

    class COREAuton {
    public:
        COREAuton(string name, bool defaultAuton = false);
        void putToDashboard(SendableChooser<COREAuton*>* chooser);

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
        virtual ~COREAuton();
    protected:
        void addFirstNode(Node* firstNode);
        virtual void addNodes() = 0;
    private:
        string m_name;
        bool m_defaultAuton = false;
        vector<Node*> m_firstNode;
    };
}
