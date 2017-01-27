#include "COREDashboard.h"


CORE::COREDashboard::COREDashboard(string name) {
    m_name = name;
}

void CORE::COREDashboard::putNumber(double value) {
    SmartDashboard::PutNumber(m_name, value);
}

double CORE::COREDashboard::getNumber() {
    return SmartDashboard::GetNumber(m_name, m_defaultNumber);
}

void CORE::COREDashboard::putString(string value) {
    SmartDashboard::PutString(m_name, value);
}

string CORE::COREDashboard::getString() {
    return SmartDashboard::GetString(m_name, m_defaultString);
}

void CORE::COREDashboard::putBoolean(bool value) {
    SmartDashboard::PutBoolean(m_name, value);
}

bool CORE::COREDashboard::getBoolean() {
    return SmartDashboard::GetBoolean(m_name, m_defaultBoolean);
}

string CORE::COREDashboard::getName() {
    return m_name;
}

void CORE::COREDashboard::setDefaultNumber(double value) {
    m_defaultNumber = value;
}

void CORE::COREDashboard::setDefaultString(string value) {
    m_defaultString = value;
}

void CORE::COREDashboard::setDefaultBoolean(bool value) {
    m_defaultBoolean = value;
}
