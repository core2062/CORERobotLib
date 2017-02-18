#include "COREController.h"

using namespace CORE;

void COREMotionController::Set(double setPoint) {
    m_setPoint = setPoint;
}

double COREMotionController::Get() {
    return m_setPoint;
}

void COREMotionController::setActual(double actualPosition) {
    m_actualPosition = actualPosition;
}

double COREMotionController::getActual() {
    return m_actualPosition;
}
