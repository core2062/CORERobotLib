#include "CORETask.h"
#include "COREFramework/COREScheduler.h"

using namespace CORE;

CORETask::CORETask() : m_disabled(false) {
	COREScheduler::addTask(this);
}

void CORETask::disableTasks(bool disable) {
	m_disabled = disable;
}

bool CORETask::isDisabled() {
	return m_disabled;
}
