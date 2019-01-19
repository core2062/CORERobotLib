#include "COREController.h"

using namespace CORE;

void COREMotionController::Set(double setPoint) {
    m_setPoint = setPoint;
}

double COREMotionController::Get() {
    return m_setPoint;
}

void COREMotionController::SetActual(double actualPosition) {
    m_actualPosition = actualPosition;
}

double COREMotionController::GetActual() {
    return m_actualPosition;
}
