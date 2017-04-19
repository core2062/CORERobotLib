#include "COREDataLog.h"

using namespace CORE;


//template<class T>
//COREDataPoint<T>::COREDataPoint(T var) {
//	m_value = new T(var);
//}
//
//template<class T>
//COREDataPoint<T>::COREDataPoint(T* var) {
//	m_value = var;
//}
//
//template<class T>
//std::string COREDataPoint<T>::getValue() {
//	return std::to_string(*m_value);
//}
//
//template<>
//std::string COREDataPoint<std::string>::getValue() {
//	return *m_value;
//}
//
//template<>
//std::string COREDataPoint<bool>::getValue() {
//	return (*m_value)?"True":"False";
//}
//
//template<>
//std::string COREDataPoint<Timer>::getValue() {
//	return std::to_string(m_value->Get());
//}
//
//template<class T>
//void COREDataPoint<T>::setValue(T const& val) {
//	delete m_value;
//	m_value = new T(val);
//}
//
std::string CORETimeDataPoint::getValue() {
	return std::to_string(Timer::GetFPGATimestamp());
}

COREDataLogger::COREDataLogger(
		std::initializer_list<std::string> headers) {
	std::string firstLine;
	for (auto i : headers){
		firstLine += i;
		firstLine += ',';
	}
	firstLine.erase(firstLine.end() - 1);
	m_lines.push_back(firstLine);
}

void COREDataLogger::putData(std::initializer_list<ICOREDataPoint*> data) {
	std::string newLine;
	for (auto i : data){
		newLine += i->getValue();
		newLine += ',';
	}
	newLine.erase(newLine.end() - 1);
	m_lines.push_back(newLine);
}

bool COREDataLogger::save(std::string filename) {
	std::string fullName = "/media/sda1/COREDataLogs/" + filename;
	std::remove(fullName.c_str());
	std::ofstream outputFile;
	outputFile.open(fullName, std::ofstream::trunc);
	if(outputFile.is_open()){
		for(auto i : m_lines){
			outputFile << i;
			outputFile << "\r\n";
			std::cout << i; std::cout << "\r\n";
		}
		outputFile.close();
		return true;
	}else{
		std::cout << "3130" << std::endl;
		return false;
	}
}


COREContinuousLogger::COREContinuousLogger(
		std::initializer_list<std::string> headers,
		std::initializer_list<ICOREDataPoint*> datas, int counts) : COREDataLogger(headers){
	m_datas = datas;
	m_counterStart = counts;
	m_counter = m_counterStart;
}

void COREContinuousLogger::postLoopTask() {
	if(m_counter <= 0){
		std::string newLine;
		for (auto i : m_datas){
			newLine += i->getValue();
			newLine += ',';
		}
		newLine.erase(newLine.end() - 1);
		m_lines.push_back(newLine);
		m_counter = m_counterStart;
	}else{
		m_counter--;
	}
}

