#include "COREConstant.h"

using namespace CORE;

vector<ICOREConstant*> COREConstantsManager::m_constants;

void COREConstantsManager::robotInit() {
    updateConstants();
}

void COREConstantsManager::updateConstants() {
    for(auto constant : m_constants) {
        constant->updateConstant();
    }
}

void COREConstantsManager::addConstant(ICOREConstant* instance) {
    m_constants.push_back(instance);
}