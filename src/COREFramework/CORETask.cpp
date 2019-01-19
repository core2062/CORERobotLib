#include "CORETask.h"
#include "COREFramework/COREScheduler.h"

using namespace CORE;

CORETask::CORETask() : m_disabled(false) {
	COREScheduler::AddTask(this);
}

void CORETask::DisableTasks(bool disable) {
	m_disabled = disable;
}

bool CORETask::IsDisabled() {
	return m_disabled;
}
