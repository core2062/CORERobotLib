#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

#include "COREAuton.h"
#include "COREHardwareManager.h"
#include "COREUtilities/COREConstant.h"
#include "CORETask.h"
#include "CORELogging/CORELog.h"
#include "COREUtilities/CORENamedObject.h"

using namespace std;
using namespace std::placeholders;

using namespace CORE;

namespace CORE {
    class COREAuton;

    class CORESubsystem : public CORENamedObject {
    public:
        CORESubsystem();
        virtual void robotInit() = 0;
        virtual void teleopInit() = 0;
        virtual void teleop() = 0;
        virtual void teleopEnd() {}
		virtual void test() {}
		virtual void disabled() {}
        virtual ~CORESubsystem() {}
    };

	class COREController : public CORETask {
	public:
		COREController();

		virtual void enabledLoop() = 0;
		virtual void enable() {
			m_enabled = true;
		}
		virtual void disable() {
			m_enabled = false;
		}
		bool isEnabled() {
			return m_enabled;
		}
		virtual ~COREController() {}
	protected:
		bool m_enabled = false;
	};

	class COREVariableControlledSubsystem : public CORESubsystem {
	public:
		COREVariableControlledSubsystem();
		virtual void robotInit() = 0;
		virtual void teleopInit() = 0;
		virtual void teleop() override;
		virtual void teleopEnd() {}
		virtual void test() {}
		virtual void disabled() {}
		bool setController(COREController * controller);
		virtual ~COREVariableControlledSubsystem(){}

	protected:
		COREController* m_currentController = 0;
	};

	class COREScheduler {
	private:
		static vector<COREAuton*> m_autons;
		static vector<CORETask*> m_tasks;
		static vector<function<void()>> m_robotInitCallBacks;
		static vector<function<void()>> m_autonInitCallBacks;
		static vector<function<void()>> m_autonEndCallBacks;
		static vector<function<void()>> m_teleopInitCallBacks;
		static vector<function<void()>> m_preLoopCallBacks;
		static vector<function<void()>> m_postLoopCallBacks;
		static vector<function<void()>> m_teleopEndCallBacks;
		static vector<function<void()>> m_disabledCallBacks;
//		static SendableChooser<COREAuton*>* m_autonChooser;
		static vector<CORESubsystem*> m_subsystems;
		static COREAuton* m_selectedAuton;
		static CORETimer m_autonTimer;
	public:
		static void addSubsystem(CORESubsystem* subsystem);
		static void addAuton(COREAuton * auton);
		static void addTask(CORETask* task);

		template<class T>
		static void addRobotInitCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addAutonInitCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addAutonEndCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addTeleopInitCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addPreLoopCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addPostLoopCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addTeleopEndCallBack(T* const object, void(T::* const callBack)());

		template<class T>
		static void addDisabledCallBack(T* const object, void(T::* const callBack)());

		static void robotInit();
		static void autonInit();
		static bool auton();
		static void autonEnd();
		static void teleopInit();
		static void teleop();
		static void teleopEnd();
        static void preLoop();
        static void postLoop();
		static void disabled();
		static void test();
		static void cleanUp();
	};
}
