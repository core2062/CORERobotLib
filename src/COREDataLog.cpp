#include "COREDataLog.h"


CORE::COREDataLog::COREDataLog(string name) {
    m_name = name;
}

void CORE::COREDataLog::putNumber(double value) {
    SmartDashboard::PutNumber(m_name, value);
}

double CORE::COREDataLog::getNumber() {
    return SmartDashboard::GetNumber(m_name, m_defaultNumber);
}

void CORE::COREDataLog::putString(string value) {
    SmartDashboard::PutString(m_name, value);
}

string CORE::COREDataLog::getString() {
    return SmartDashboard::GetString(m_name, m_defaultString);
}

void CORE::COREDataLog::putBoolean(bool value) {
    SmartDashboard::PutBoolean(m_name, value);
}

bool CORE::COREDataLog::getBoolean() {
    return SmartDashboard::GetBoolean(m_name, m_defaultBoolean);
}

string CORE::COREDataLog::getName() {
    return m_name;
}

void CORE::COREDataLog::setDefaultNumber(double value) {
    m_defaultNumber = value;
}

void CORE::COREDataLog::setDefaultString(string value) {
    m_defaultString = value;
}

void CORE::COREDataLog::setDefaultBoolean(bool value) {
    m_defaultBoolean = value;
}
