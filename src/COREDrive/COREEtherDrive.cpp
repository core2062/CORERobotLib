#include "COREEtherDrive.h"

using namespace CORE;

double etherL(double fwd, double rcw, double a, double b){
    return fwd + b*rcw*(1-fwd);
}

double etherR(double fwd, double rcw, double a, double b){
    return fwd-b*rcw + fwd*rcw*(b-a-1);
}

void COREEtherDrive::update() {
    if (drive_mag>0){
        if (drive_rot>=0){
            left = etherL(drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))? 1: .5, drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
            right = etherR(drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
        } else{
            left = etherR(drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, -drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
            right = etherL(drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, -drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
        }
    } else if (drive_mag < 0) {
        if (drive_rot>=0){

            left = -etherR(-drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
            right = -etherL(-drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
        } else{
            left = -etherL(-drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, -drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
            right = -etherR(-drive_mag * (robot.joystick.button(DRIVE_MAG_SPEED))?1:.5, -drive_rot, a, b * (robot.joystick.button(DRIVE_ROT_SPEED)?2:1));
        }
    } else {
        left = (drive_rot * (robot.joystick.button(DRIVE_ROT_SPEED) ? 1 : SmartDashboard::GetNumber(quickTurn.n,quickTurn.v)));
        right = (-drive_rot * (robot.joystick.button(DRIVE_ROT_SPEED) ? 1 : SmartDashboard::GetNumber(quickTurn.n,quickTurn.v)));
    }
}
