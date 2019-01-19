#include "COREConstant.h"

using namespace CORE;

vector<ICOREConstant*> COREConstantsManager::m_constants;

void COREConstantsManager::UpdateConstants() {
    for(auto constant : m_constants) {
        constant->UpdateConstant();
    }
}

void COREConstantsManager::AddConstant(ICOREConstant* instance) {
    m_constants.push_back(instance);
}

void COREConstantsManager::CleanUp() {
	for (auto &constant : m_constants) {
		delete constant;
	}
	m_constants.clear();
}
