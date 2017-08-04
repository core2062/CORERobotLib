#include "COREConstant.h"

using namespace CORE;

vector<ICOREConstant*> COREConstantsManager::m_constants;

void COREConstantsManager::updateConstants() {
    for(auto constant : m_constants) {
        constant->updateConstant();
    }
}

void COREConstantsManager::addConstant(ICOREConstant* instance) {
    m_constants.push_back(instance);
}

void COREConstantsManager::cleanUp() {
	for (auto i = m_constants.begin(); i != m_constants.end(); i++){
		delete *i;
	}
	m_constants.clear();
}
