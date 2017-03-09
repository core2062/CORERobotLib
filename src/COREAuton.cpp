#include "COREAuton.h"

using namespace CORE;

/*
 * Create a node with given actions and timeout. These actions will all be run in parallel to each other.
 * Nullptrs will not be added as actions, they will be ignored.
 */
Node::Node(double timeout, COREAutonAction* action1, COREAutonAction* action2, COREAutonAction* action3) :
        m_children(), m_actions(), m_timeout(timeout) {
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
void Node::addNext(Node* childNode) {
    m_children.push_back(childNode);
}

/*
 * Add an action to this node which will be run in parallel to other actions in this node
 */
void Node::addAction(COREAutonAction* leaf) {
    if(leaf != nullptr) {
        m_actions.push_back(leaf);
    }
}

/*
 * Add a condition to this node.
 * This condition will be checked by the previous node while it is not complete.
 * While it is true this action will run its actions.
 */
void Node::addCondition(bool(* startCondition)()) {
    m_startConditonGiven = true;
    m_startCondition = startCondition;
}

/*
 * Returns true if all actions are complete and all child nodes are complete or if timeout has been exceeded
 */
bool Node::complete() {
    if(m_actions.empty()) {
        for(auto child : m_children) {
            if(!child->complete()) {
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
void Node::reset() {
    for(auto child : m_children) {
        child->reset();
    }
    m_actions.clear();
    m_children.clear();
    m_actionsInitialized = false;
}

/*
 * Set the timeout for this node to a given duration in seconds
 */
void Node::setTimeout(double timeout) {
    m_timeout = timeout;
}

/**
 * Runs all actions in node and checks if child node conditions are true. If they are and this node is not done, that
 * child node is run.
 * @param lastNodeDone If previous node is complete.
 */
void Node::act(bool lastNodeDone) {
    bool shouldAct = (m_startConditonGiven && m_startCondition && !lastNodeDone)
                     || (!m_startConditonGiven && lastNodeDone);
    if(!m_actionsInitialized && shouldAct) {

        m_timer.Reset();
        m_timer.Start();
        std::cout << "Timeout Timer Started" << std::endl;

        for(auto action : m_actions) {
            action->actionInit();
        }
        m_actionsInitialized = true;
    }
    if(!m_actions.empty() && (m_timer.Get() > m_timeout)) {
        CORELog::logInfo("Timeout of: " + to_string(m_timeout) + " exceeded in node!");
        for(auto action : m_actions) {
            action->actionEnd();
        }
        for (auto i = m_actions.begin(); i != m_actions.end(); i++){
            delete *i;
        }
        m_actions.clear();
    }
    if(!m_actions.empty()) {
        if(shouldAct) {
            for(unsigned int i = 0; i < m_actions.size(); i++) {
                COREAutonAction::actionStatus status = m_actions[i]->action();
                switch(status) {
                    case COREAutonAction::END:
                        m_actions[i]->actionEnd();
                        //TODO: There's a memory leak here, need to fix
                        m_actions.erase(m_actions.begin() + i);
                        i--;
                        break;
                    case COREAutonAction::CONTINUE:
                        break;
                }
            }
            for(auto child : m_children) {
                child->act(false);
            }
        } else if(m_startConditonGiven && !m_startCondition) {
            return;
        }
    } else {
    	for(auto child : m_children) {
    		child->act(true);
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
    COREScheduler::addAuton(this);
}

/*
 * Run autonomous routine. Needs to be called each iteration.
 */
void COREAuton::auton() {
    for(auto node : m_firstNode) {
        node->act(true);
    }
}

/*
 * Initialize autonomous routine by reseting and adding all nodes.
 */
void COREAuton::autonInit() {
    reset();
    addNodes();
}

/*
 * Put this autonomous routine to Smart Dashboard on a given SendableChooser
 */
void COREAuton::putToDashboard(shared_ptr<SendableChooser<COREAuton*>> chooser) {
    CORELog::logInfo("Adding autonomous: " + m_name + " to dashboard");
    if(m_defaultAuton) {
        chooser->AddDefault(m_name, this);
    } else {
        chooser->AddObject(m_name, this);
    }
}

/*
 * If autonomous routine is complete
 */
bool COREAuton::complete() {
    for(auto node : m_firstNode) {
        if(!node->complete()) {
            return false;
        }
    }
    return true;
}

/*
 * Reset all nodes in autonomous routine
 */
void COREAuton::reset() {
    for(auto node : m_firstNode) {
        if(node) {
            node->reset();
        }
    }
}

/*
 * Add another node to be run on start of autonomous routine
 * Nullptrs will not be added as actions, they will be ignored
 */
void COREAuton::addFirstNode(Node* firstNode) {
    if(firstNode != nullptr) {
        m_firstNode.push_back(firstNode);
    }
}

/*
 * Returns human readable name of routine
 */
string COREAuton::getName() {
    return m_name;
}

COREAuton::~COREAuton() {
    for (auto i = m_firstNode.begin(); i != m_firstNode.end(); i++){
        delete *i;
    }
    m_firstNode.clear();
}


