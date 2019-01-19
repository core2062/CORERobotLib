#pragma once

#include <vector>
#include <fstream>
#include <iostream>
//#include "COREFramework/CORETask.h"
#include "COREUtilities/CORETimer.h"

using namespace std;
using namespace CORE;

namespace CORE {

	class ICOREDataPoint{
	public:
		virtual std::string GetValue(){return "?";};
		virtual ~ICOREDataPoint(){};
	};

	template<class T>
	class COREDataPoint : public ICOREDataPoint{
	public:
		COREDataPoint(T var){
			m_value = new T(var);
		}
		COREDataPoint(T * var);

		std::string GetValue();
		void SetValue(T val);
	private:
		T * m_value;
	};

	template<>
	std::string COREDataPoint<std::string>::GetValue();

	template<>
	std::string COREDataPoint<bool>::GetValue();

	template<>
	std::string COREDataPoint<CORETimer>::GetValue();

	class CORETimeDataPoint : public ICOREDataPoint{
	public:
		std::string GetValue() override;
	};

	class COREDataLogger{
	public:
		COREDataLogger(std::initializer_list<std::string> headers);

		void PutData(std::initializer_list<ICOREDataPoint*> data);
		bool Save(std::string filename);
	protected:
		std::vector<std::string> m_lines;
	};

	class COREContinuousLogger : public COREDataLogger{
	public:
		COREContinuousLogger(std::initializer_list<std::string> headers,
				std::initializer_list<ICOREDataPoint*> datas, int counts);
		void PostLoopTask();
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
inline std::string CORE::COREDataPoint<T>::GetValue() {
	return std::to_string(*m_value);
}

template<class T>
inline void CORE::COREDataPoint<T>::SetValue(T val) {
	delete m_value;
	m_value = new T(val);
}

template<>
inline std::string CORE::COREDataPoint<std::string>::GetValue() {
	return *m_value;
}

template<>
inline std::string CORE::COREDataPoint<bool>::GetValue() {
	return (*m_value)?"True":"False";
}

template<>
inline std::string CORE::COREDataPoint<CORETimer>::GetValue() {
	return std::to_string(m_value->Get());
}
