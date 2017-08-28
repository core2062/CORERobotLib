#include "CORENamedObject.h"

using namespace CORE;

/**
 * Get a human readable version of the name of this object. Needs to be set in update name first
 * @return Human readable object name
 */
string CORE::CORENamedObject::getName() {
    return m_name;
}

/**
 * Update the name of this object. Preformed automaticly on robotInit().
 *
 * Uses Typeid and a demangler to find human readable name of child object.
 * More info: https://stackoverflow.com/a/10896310
 */

void CORE::CORENamedObject::robotInitTask() {
    m_name = typeid(*this).name();
#ifdef __GNUG__
        int status;
        char* temp = __cxxabiv1::__cxa_demangle(m_name.c_str(), nullptr, nullptr, &status);
        if(temp) {
            string result(temp);
            free(temp);
            m_name = result;
        }
#endif
    CORELog::logError(m_name);
}