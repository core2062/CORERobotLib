#include "COREAuton.h"

using namespace CORE;
using namespace std;

/*
 * Create a node with given actions and timeout. These actions will all be run in parallel to each other.
 * Nullptrs will not be added as actions, they will be ignored.
 */
Node::Node(double timeout, COREAutonAction* action1, COREAutonAction* action2, COREAutonAction* action3) :
        m_children(), m_actions(), m_timeout(timeout) {
	m_startCondition = nullptr;
    if(action1 != nullptr) {
        m_actions.push_back(action1);
    }
    if(action2 != nullptr) {
        m_actions.push_back(action2);
    }
    if(action3 != nullptr) {
        m_actions.push_back(action3);
    }
}

/*
 * Add a node to this node which will be run sequentially after this node completes all of its actions
 */
void Node::AddNext(Node* childNode) {
    m_children.push_back(childNode);
}

/*
 * Add an action to this node which will be run in parallel to other actions in this node
 */
void Node::AddAction(COREAutonAction* leaf) {
    if(leaf != nullptr) {
        m_actions.push_back(leaf);
    }
}

/*
 * Add a condition to this node.
 * This condition is passed in as a std::function that will be called to check the condition.
 *
 * Typically, the best way to do this is to create a lambda that contains the code to see if the node should be run.
 *
 * Example:
 * addCondition([]{return shouldNodeRun()});
 *
 * This condition will be checked by the previous node while it is not complete.
 * While it is true this action will run its actions.
 */
void Node::AddCondition(function<bool()> startCondition) {
    m_startConditonGiven = true;
    m_startCondition = startCondition;
}

/*
 * Returns true if all actions are complete and all child nodes are complete or if timeout has been exceeded
 */
bool Node::Complete() {
    if(m_actions.empty()) {
        for(auto child : m_children) {
            if(!child->Complete()) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

/*
 * Prepares node to be run
 */
void Node::Reset() {
    for(auto child : m_children) {
        child->Reset();
    }
    m_actions.clear();
    m_children.clear();
    m_actionsInitialized = false;
}

/*
 * Set the timeout for this node to a given duration in seconds
 */
void Node::SetTimeout(double timeout) {
    m_timeout = timeout;
}

/**
 * Runs all actions in node and checks if child node conditions are true. If they are and this node is not done, that
 * child node is run.
 * @param lastNodeDone If previous node is complete.
 */
void Node::Act(bool lastNodeDone) {
    bool shouldAct = (m_startConditonGiven && m_startCondition && !lastNodeDone)
                     || (!m_startConditonGiven && lastNodeDone);
    if(!m_actionsInitialized && shouldAct) {

        m_timer.Reset();
        m_timer.Start();
        std::cout << "Timeout Timer Started" << std::endl;

        for(auto action : m_actions) {
            action->ActionInit();
        }
        m_actionsInitialized = true;
    }
    if(!m_actions.empty() && (m_timer.Get() > m_timeout)) {
        CORELog::LogInfo("Timeout of: " + to_string(m_timeout) + " exceeded in node!");
        for(auto action : m_actions) {
            action->ActionEnd();
        }
        for (auto i = m_actions.begin(); i != m_actions.end(); i++){
            delete *i;
        }
        m_actions.clear();
    }
    if(!m_actions.empty()) {
        if(shouldAct) {
            for(unsigned int i = 0; i < m_actions.size(); i++) {
                COREAutonAction::actionStatus status = m_actions[i]->Action();
                switch(status) {
                    case COREAutonAction::END:
                        m_actions[i]->ActionEnd();
                        //TODO: There's a memory leak here, need to fix
                        m_actions.erase(m_actions.begin() + i);
                        i--;
                        break;
                    case COREAutonAction::CONTINUE:
                        break;
                }
            }
            for(auto child : m_children) {
                child->Act(false);
            }
        } else if(m_startConditonGiven && !m_startCondition) {
            return;
        }
    } else {
    	for(auto child : m_children) {
    		child->Act(true);
    	}
    }
}

Node::~Node() {
    for (auto i = m_actions.begin(); i != m_actions.end(); i++){
        delete *i;
    }
    m_actions.clear();
    for (auto i = m_children.begin(); i != m_children.end(); i++){
        delete *i;
    }
    m_children.clear();
}

/*
 * Initialize an autonomous routine with given name, first node, and whether this autonomous should be the default
 * selected autonomous on Smart Dashboard
 */
COREAuton::COREAuton(string name, bool defaultAuton) {
    m_name = name;
    m_defaultAuton = defaultAuton;
    COREScheduler::AddAuton(this);
}

/*
 * Run autonomous routine. Needs to be called each iteration.
 */
void COREAuton::Auton() {
    for(auto node : m_firstNode) {
        node->Act(true);
    }
}

/*
 * Initialize autonomous routine by resetting and adding all nodes.
 */
void COREAuton::AutonInit() {
    Reset();
    AddNodes();
}

/*
 * Put this autonomous routine to Smart Dashboard on a given SendableChooser
 */
void COREAuton::PutToDashboard(frc::SendableChooser<COREAuton*>* chooser) {
    CORELog::LogInfo("Adding autonomous: " + m_name + " to dashboard");
    if(m_defaultAuton) {
        chooser->SetDefaultOption(m_name, this);
    } else {
        chooser->SetDefaultOption(m_name, this);
    }
}

/*
 * If autonomous routine is complete
 */
bool COREAuton::Complete() {
    for(auto node : m_firstNode) {
        if(!node->Complete()) {
            return false;
        }
    }
    return true;
}

/*
 * Reset all nodes in autonomous routine
 */
void COREAuton::Reset() {
    for(auto node : m_firstNode) {
        if(node != nullptr) {
            node->Reset();
        }
    }
}

/*
 * Add another node to be run on start of autonomous routine
 * Nullptrs will not be added as actions, they will be ignored
 */
void COREAuton::AddFirstNode(Node* firstNode) {
    if(firstNode != nullptr) {
        m_firstNode.push_back(firstNode);
    }
}

/*
 * Returns human readable name of routine
 */
string COREAuton::GetName() {
    return m_name;
}

/*
 * Automatically deletes all nodes and their actions on deletion of the autonomous routine
 */
COREAuton::~COREAuton() {
    for (auto i = m_firstNode.begin(); i != m_firstNode.end(); i++){
        delete *i;
    }
    m_firstNode.clear();
}

/*
 * Takes a duration in seconds to wait for.
 */
WaitAction::WaitAction(double duration) {
	m_duration = duration;
}

void WaitAction::ActionInit() {
	m_timer.Reset();
	m_timer.Start();
}

COREAutonAction::actionStatus WaitAction::Action() {
	if(m_timer.Get() > m_duration) {
		return COREAutonAction::END;
	} else {
		return COREAutonAction::CONTINUE;
	}
}


