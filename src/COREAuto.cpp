#include "COREAuto.h"

using namespace CORE;

Node::Node(COREAutoAction *action1, COREAutoAction *action2, COREAutoAction *action3):
m_children(), m_actions() {
    shared_ptr<COREAutoAction> pointer(action1);
    m_actions.push_back(pointer);
    if(action2 != nullptr) {
        shared_ptr<COREAutoAction> pointer(action2);
        m_actions.push_back(pointer);
    } else if (action3 != nullptr) {
        shared_ptr<COREAutoAction> pointer(action3);
        m_actions.push_back(pointer);
    }
}

Node::Node(shared_ptr<COREAutoAction> action1, shared_ptr<COREAutoAction> action2,
        shared_ptr<COREAutoAction> action3) : m_children(), m_actions() {
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

void Node::addAction(COREAutoAction *leaf) {
    shared_ptr<COREAutoAction> pointer(leaf);
    m_actions.push_back(pointer);
}

void Node::addAction(shared_ptr<COREAutoAction> leaf) {
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

COREAuto::COREAuto() {
    shared_ptr<COREAuto> pointer(this);
    m_instance = pointer;
    COREScheduler::addAuton(m_instance);
}

void COREAuto::auton() {
    for (auto node : m_firstNode) {
        node->act(true);
    }
}

void COREAuto::autonInit() {
    addNodes();
}

void COREAuto::putToDashboard(shared_ptr<SendableChooser> chooser) {
    cout << "Adding Autonomous: " << m_name << " to dashboard" << endl;
    if(m_defaultAuton) {
        chooser->AddDefault(m_name, m_instance.get());
    }
    else {
        chooser->AddObject(m_name, m_instance.get());
    }
}

bool COREAuto::complete() {
    bool nodesComplete = true;
    for(auto node : m_firstNode) {
        nodesComplete = node->complete();
    }
    return nodesComplete;
}

void COREAuto::addFirstNode(Node *firstNode) {
    m_firstNode.push_back(firstNode);
}

void COREAuto::setName(string name) {
    m_name = name;
}

void COREAuto::setDefault(bool defaultAuton) {
    m_defaultAuton =  defaultAuton;
}
