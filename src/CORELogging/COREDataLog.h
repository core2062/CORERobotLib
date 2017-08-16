#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "COREFramework/CORETask.h"
#include "COREUtilities/CORETimer.h"

using namespace std;
using namespace CORE;

namespace CORE {

	class ICOREDataPoint{
	public:
		virtual std::string getValue(){return "?";};
		virtual ~ICOREDataPoint(){};
	};

	template<class T>
	class COREDataPoint : public ICOREDataPoint{
	public:
		COREDataPoint(T var){
			m_value = new T(var);
		}
		COREDataPoint(T * var);

		std::string getValue();
		void setValue(T val);
	private:
		T * m_value;
	};

	template<>
	std::string COREDataPoint<std::string>::getValue();

	template<>
	std::string COREDataPoint<bool>::getValue();

	template<>
	std::string COREDataPoint<CORETimer>::getValue();

	class CORETimeDataPoint : public ICOREDataPoint{
	public:
		std::string getValue() override;
	};

	static CORETimeDataPoint matchTime;

	class COREDataLogger{
	public:
		COREDataLogger(std::initializer_list<std::string> headers);

		void putData(std::initializer_list<ICOREDataPoint*> data);
		bool save(std::string filename);
	protected:
		std::vector<std::string> m_lines;
	};

	class COREContinuousLogger : public COREDataLogger, CORETask{
	public:
		COREContinuousLogger(std::initializer_list<std::string> headers,
				std::initializer_list<ICOREDataPoint*> datas, int counts);
		void postLoopTask();
	private:
		std::vector<ICOREDataPoint*> m_datas;
		int m_counter;
		int m_counterStart;
	};
}

template<class T>
inline CORE::COREDataPoint<T>::COREDataPoint(T* var) {
	m_value = var;
}

template<class T>
inline std::string CORE::COREDataPoint<T>::getValue() {
	return std::to_string(*m_value);
}

template<class T>
inline void CORE::COREDataPoint<T>::setValue(T val) {
	delete m_value;
	m_value = new T(val);
}

template<>
inline std::string CORE::COREDataPoint<std::string>::getValue() {
	return *m_value;
}

template<>
inline std::string CORE::COREDataPoint<bool>::getValue() {
	return (*m_value)?"True":"False";
}

template<>
inline std::string CORE::COREDataPoint<CORETimer>::getValue() {
	return std::to_string(m_value->Get());
}
