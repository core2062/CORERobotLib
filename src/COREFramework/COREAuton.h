#pragma once

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include <iostream>

#include <WPILib.h>
#include "COREUtilities/CORETimer.h"
#include "COREScheduler.h"
#include "CORELogging/CORELog.h"

using namespace std;
namespace CORE {
    class COREAutonAction {
    public:
        enum actionStatus {
            CONTINUE,
            END
        };

        virtual void ActionInit() {}

        virtual void ActionEnd() {}

        virtual actionStatus Action() = 0;

        virtual ~COREAutonAction() {}
    };

    class Node {
    public:
        Node(double timeout, COREAutonAction* action1 = nullptr, COREAutonAction* action2 = nullptr, COREAutonAction* action3 = nullptr);
        void AddNext(Node* childNode);
        void AddAction(COREAutonAction* leaf);
        void AddCondition(function<bool()> startCondition);
        bool Complete();
        void Reset();
        void SetTimeout(double timeout);
        void Act(bool lastNodeDone);
        ~Node();
    private:
        vector<Node*> m_children;
        vector<COREAutonAction*> m_actions;
        bool m_startConditonGiven = false;
        bool m_actionsInitialized = false;
        function<bool()> m_startCondition;
        double m_timeout;
        CORETimer m_timer;
    };

    class COREAuton {
    public:
        COREAuton(string name, bool defaultAuton = false);
        void PutToDashboard(SendableChooser<COREAuton*>* chooser);

        inline COREAuton* GetInstance() {
            return this;
        }

        inline bool GetDefault() {
            return m_defaultAuton;
        }

        string GetName();
        void Auton();
        void AutonInit();
        bool Complete();
        void Reset();
        virtual ~COREAuton();
    protected:
        void AddFirstNode(Node* firstNode);
        virtual void AddNodes() = 0;
    private:
        string m_name;
        bool m_defaultAuton = false;
        vector<Node*> m_firstNode;
    };

    class WaitAction : public COREAutonAction {
    public:
    	WaitAction(double duration);
    	void ActionInit();
    	actionStatus Action();
    private:
    	CORETimer m_timer;
    	double m_duration;
    };
}
