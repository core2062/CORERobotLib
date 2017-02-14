#include "COREController.h"


void CORE::COREMotionController::Set(double setPoint) {
    m_setPoint = setPoint;
}

double CORE::COREMotionController::Get() {
    return m_setPoint;
}

void CORE::COREMotionController::setActual(double actualPosition) {
    m_actualPosition = actualPosition;
}

double CORE::COREMotionController::getActual() {
    return m_actualPosition;
}
