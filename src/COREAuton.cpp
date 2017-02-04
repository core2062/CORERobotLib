#include "COREAuton.h"

using namespace CORE;

Node::Node(COREAutonAction *action1, COREAutonAction *action2, COREAutonAction *action3):
m_children(), m_actions() {
    shared_ptr<COREAutonAction> pointer(action1);
    m_actions.push_back(pointer);
    if(action2 != nullptr) {
        shared_ptr<COREAutonAction> pointer(action2);
        m_actions.push_back(pointer);
    } else if (action3 != nullptr) {
        shared_ptr<COREAutonAction> pointer(action3);
        m_actions.push_back(pointer);
    }
}

Node::Node(shared_ptr<COREAutonAction> action1, shared_ptr<COREAutonAction> action2,
        shared_ptr<COREAutonAction> action3) : m_children(), m_actions() {
    m_actions.push_back(action1);
    if(action2 != nullptr) {
        m_actions.push_back(action2);
    } else if (action3 != nullptr) {
        m_actions.push_back(action3);
    }
}

void Node::addNext(Node *childNode) {
    shared_ptr<Node> pointer(childNode);
    m_children.push_back(pointer);
}

void Node::addNext(shared_ptr<Node> childNode) {
    m_children.push_back(childNode);
}

void Node::addAction(COREAutonAction *leaf) {
    shared_ptr<COREAutonAction> pointer(leaf);
    m_actions.push_back(pointer);
}

void Node::addAction(shared_ptr<COREAutonAction> leaf) {
    m_actions.push_back(leaf);
}

void Node::addCondition(bool(*startCondition)()) {
    m_startConditonGiven = true;
    m_startCondition = startCondition;
}

bool Node::complete() {
    if(m_actions.empty()) {
        bool childrenComplete = true;
        for(auto child : m_children) {
            childrenComplete = child->complete();
        }
        return childrenComplete;
    }
    else {
        return false;
    }
}

void Node::act(bool lastNodeDone) {
    if(!m_actionsInitialized) {
        for(auto action : m_actions) {
            action->actionInit();
            m_actionsInitialized = true;
        }
    }
    if(!m_actions.empty()) {
        if ((m_startConditonGiven && m_startCondition) || (!m_startConditonGiven && lastNodeDone)) {
            for (int i = 0; i <  m_actions.size(); i++) {
                actionStatus status = m_actions[i]->action();
                switch (status) {
                    case END:
                        m_actions[i]->actionEnd();
                        m_actions.erase(m_actions.begin() + i);
                        i--;
                        break;
                    case CONTINUE:
                        break;
                }
            }
            for (auto child : m_children) {
                child->act(false);
            }
        }
        else if (m_startConditonGiven && !m_startCondition) {
            return;
        }
    }
    else {
        for (auto child : m_children) {
            child->act(true);
        }
    }
}

COREAuton::COREAuton(string name, Node * firstNode, bool defaultAuton) {
    m_name = name;
    m_defaultAuton = defaultAuton;
    m_firstNode.push_back(firstNode);
    COREScheduler::addAuton(this);
}

void COREAuton::auton() {
    for (auto node : m_firstNode) {
        node->act(true);
    }
}

void COREAuton::autonInit() {
    addNodes();
}

void COREAuton::putToDashboard(shared_ptr<SendableChooser<COREAuton*>> chooser) {
    CORELog::logInfo("Adding Autonomous: " + m_name + " to dashboard");
    if(m_defaultAuton) {
        chooser->AddDefault(m_name, this);
    }
    else {
        chooser->AddObject(m_name, this);
    }
}

bool COREAuton::complete() {
    bool nodesComplete = true;
    for(auto node : m_firstNode) {
        nodesComplete = node->complete();
    }
    return nodesComplete;
}

void COREAuton::addFirstNode(Node * firstNode) {
    m_firstNode.push_back(firstNode);
}

string COREAuton::getName() {
    return m_name;
}
