/* CORERobotLib
 * FRC Team CORE 2062
 *
 * Created by:
 * Andrew Kempen (akempen001 at gmail.com)
 *
 * Contributors:
 *
 */

#include "COREControl/COREController.h"
#include "COREControl/COREMotionProfile.h"
#include "COREControl/COREPID.h"
#include "COREDashboard/COREConnectionHandler.h"
#include "COREDashboard/COREDashboard.h"
#include "COREDashboard/COREDashboard.h"
#include "COREData/COREData.h"
#include "COREData/COREDataConnectionHandler.h"
#include "COREDrive/COREDrive.h"
#include "COREDrive/COREEtherDrive.h"
#include "COREDrive/CORESwerveDrive.h"
#include "COREFramework/COREAuton.h"
#include "COREFramework/CORERobot.h"
#include "COREFramework/COREScheduler.h"
#include "COREFramework/CORETask.h"
#include "COREHardware/COREEncoder.h"
#include "COREFramework/COREHardwareManager.h"
#include "COREHardware/COREJoystick.h"
#include "COREHardware/CORESensor.h"
#include "CORELogging/CORELog.h"
#include "COREUtilities/COREConstant.h"
#include "COREUtilities/COREMath.h"
#include "COREUtilities/CORENamedObject.h"
#include "COREUtilities/CORETimer.h"
#include "COREUtilities/Position2d.h"
#include "COREUtilities/COREVector.h"
#include "COREKinematics/AdaptivePursuit.h"
#include "COREKinematics/InterpolatingMap.h"
#include "COREKinematics/Path.h"
#include "COREKinematics/PathLoader.h"
#include "COREKinematics/PathSegment.h"
#include "COREKinematics/RobotFrame.h"
#include "COREKinematics/TankKinematics.h"
#include "COREKinematics/TankTracker.h"
#include "COREKinematics/WaypointFollower.h"
