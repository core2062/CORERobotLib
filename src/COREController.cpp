#include "COREController.h"


void CORE::COREController::Set(double setPoint) {
    m_setPoint = setPoint;
}

double CORE::COREController::Get() {
    return m_setPoint;
}

void CORE::COREController::setActual(double actualPosition) {
    m_actualPosition = actualPosition;
}

double CORE::COREController::getActual() {
    return m_actualPosition;
}
