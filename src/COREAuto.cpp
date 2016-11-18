#include "COREAuto.h"

using namespace CORE;

Node::Node(COREAutoAction *action) : children(), actions() {
    actions.push_back(action);
}

void Node::addNext(Node *childNode) {
    children.push_back(childNode);
}

void Node::addAction(COREAutoAction *leaf) {
    actions.push_back(leaf);
}

void Node::addCondition(bool(*startCondition)()) {
    m_startConditonGiven = true;
    m_startCondition = startCondition;
}

bool Node::start(bool lastNodeComplete) {
    if (m_startConditonGiven) {
        return m_startCondition();
    }
    return lastNodeComplete;
}

void Node::doActions() {
    for (auto action : actions) {
        action->action();
    }
    for (auto child : children) {
        child->doActions();
    }
}

COREAuto::COREAuto() {
    cout << "Adding to SD" << endl;
}

void COREAuto::auton() {
    addNodes();
    for (auto node : m_firstNode) {
        node->doActions();
    }
}

void COREAuto::addFirstNode(Node *firstNode) {
    m_firstNode.push_back(firstNode);
}