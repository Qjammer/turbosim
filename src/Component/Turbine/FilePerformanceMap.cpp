#include "FilePerformanceMap.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include "Turbine.hpp"
#include "../../Boundary/FluidBoundary/FluidBoundary.hpp"

double getValue(const std::map<double, std::map<double, double>>& map, double x, double y);
std::pair<double, double> getValue(const std::map<double, double>& map, double x);
double getDerivative(const std::map<double, std::map<double, double>>& map, double x, double y, int var);
std::pair<double, double> getDerivative(const std::map<double, double>& map, double x);
std::pair<double, double> interpolate(const std::pair<double, double>& begin, const std::pair<double, double>& end, double x);
double slope(const std::pair<double, double>& begin, const std::pair<double, double>& end);

FilePerformanceMap::FilePerformanceMap(
	std::string filename_efficiency, std::string filename_pi
) 
{
	{
		auto stream = std::ifstream(filename_efficiency);
		if(!stream.is_open()){
			throw std::runtime_error("Stream is not open");
		}
		this->parseFile(stream, this->efficiencyValues);
	}
	{
		auto stream = std::ifstream(filename_pi);
		if(!stream.is_open()){
			throw std::runtime_error("Stream is not open");
		}
		this->parseFile(stream, this->pressureRatioValues);
	}
}

void FilePerformanceMap::parseFile(std::ifstream& fstream, std::map<double, std::map<double, double>>& map) {
	char col_delim = '\t';
	char row_delim = '\n';
	std::string row;
	while(std::getline(fstream, row, row_delim)) {
		std::stringstream ss(row);
		std::string x_str;
		if(!std::getline(ss, x_str, col_delim)){
			continue;
		}
		std::string y_str;
		if(!std::getline(ss, y_str, col_delim)){
			continue;
		}
		std::string v_str;
		if(!std::getline(ss, v_str, row_delim)){
			continue;
		}

		double x = std::stod(x_str);
		double y = std::stod(y_str);
		double v = std::stod(v_str);

		this->insertPoint(map,x,y,v);
	}
}

void FilePerformanceMap::insertPoint(std::map<double, std::map<double, double>>& map, double x, double y, double v) {
	map[x][y] = v;
}

double FilePerformanceMap::getEfficiency(const Turbine& turbine) const {
	auto n = turbine.fwdAxle->getVelocity(turbine.fwdAxleDir);
	auto corrected_mdot =  - turbine.inlet->getCorrectedMassFlow(turbine.inletDir) * 1e5;
	auto rpm = n * 60 / (2 * M_PI);
	return getValue(this->efficiencyValues, rpm, corrected_mdot);
}
double FilePerformanceMap::getEfficiencyDerivative(const Turbine& turbine, const Parameter& p) const {
	auto n = turbine.fwdAxle->getVelocity(turbine.fwdAxleDir);
	auto dndp = turbine.fwdAxle->getVelocityDerivative(p, turbine.fwdAxleDir);
	auto mdot =  - turbine.inlet->getCorrectedMassFlow(turbine.inletDir) * 1e5;
	auto dmdotdp =  - turbine.inlet->getCorrectedMassFlowDerivative(p, turbine.inletDir) * 1e5;
	auto drpmdn = 60 / (2 * M_PI);
	auto rpm = n * drpmdn;

	auto deffdrpm = getDerivative(this->efficiencyValues, rpm, mdot, 0);
	auto deffdmdot = getDerivative(this->efficiencyValues, rpm, mdot, 1);

	auto deffdn = deffdrpm * drpmdn; 

	return deffdn * dndp + deffdmdot * dmdotdp;

}

double FilePerformanceMap::getPressureRatio(const Turbine& turbine) const {
	auto n = turbine.fwdAxle->getVelocity(turbine.fwdAxleDir);
	auto corrected_mdot =  - turbine.inlet->getCorrectedMassFlow(turbine.inletDir) * 1e5;
	auto rpm = n * 60 / (2 * M_PI);
	return getValue(this->pressureRatioValues, rpm, corrected_mdot);
}
double FilePerformanceMap::getPressureRatioDerivative(const Turbine& turbine, const Parameter& p) const {
	auto n = turbine.fwdAxle->getVelocity(turbine.fwdAxleDir);
	auto dndp = turbine.fwdAxle->getVelocityDerivative(p, turbine.fwdAxleDir);
	auto mdot =  - turbine.inlet->getCorrectedMassFlow(turbine.inletDir) * 1e5;
	auto dmdotdp =  - turbine.inlet->getMassFlowDerivative(p, turbine.inletDir) * 1e5;
	auto drpmdn = 60 / (2 * M_PI);
	auto rpm = n * drpmdn;

	auto deffdrpm = getDerivative(this->pressureRatioValues, rpm, mdot, 0);
	auto deffdmdot = getDerivative(this->pressureRatioValues, rpm, mdot, 1);

	auto deffdn = deffdrpm * drpmdn; 

	return deffdn * dndp + deffdmdot * dmdotdp;
}


double getValue(const std::map<double, std::map<double, double>>& map, double x, double y){
	auto before = map.begin();
	auto after = before;
	for(auto it = map.begin(); it != map.end(); it++){
		after = it;
		if( after->first >= x ){
			break;
		}
		before = after;
	}
	std::pair<double, double> beforeVal = {before->first, getValue(map.at(before->first), y).second};
	std::pair<double, double> afterVal = {after->first, getValue(map.at(after->first), y).second};

	return interpolate(beforeVal, afterVal, x).second;
}

std::pair<double, double> getValue(const std::map<double, double>& map, double x) {
	auto before = map.begin();
	auto after = before;
	for(auto it = map.begin(); it != map.end(); it++){
		after = it;
		if(after->first >= x) {
			break;
		}
		before = after;
	}
	return interpolate(*before, *after, x);
}


double getDerivative(const std::map<double, std::map<double, double>>& map, double x, double y, int var){
	auto before = map.begin();
	auto after = before;
	for(auto it = map.begin(); it != map.end(); it++){
		after = it;
		if( after->first >= x ){
			break;
		}
		before = after;
	}
	if(var == 0){
		std::pair<double, double> beforeVal = {before->first, getValue(map.at(before->first), y).second};
		std::pair<double, double> afterVal = {after->first, getValue(map.at(after->first), y).second};
		return slope(beforeVal, afterVal);
	} else if(var == 1) {
		std::pair<double, double> slopeBefore = {before->first, getDerivative(map.at(before->first), y).second};
		std::pair<double, double> slopeAfter = {after->first, getDerivative(map.at(after->first), y).second};
		return interpolate(slopeBefore, slopeAfter, x).second;
	} else {
		return 0.0;
	}
}
std::pair<double, double> getDerivative(const std::map<double, double>& map, double x) {
	auto before = map.begin();
	auto after = before;
	for(auto it = map.begin(); it != map.end(); it++){
		after = it;
		if(after->first >= x) {
			break;
		}
		before = after;
	}

	return {x, slope(*before, *after)};
}

std::pair<double, double> interpolate(const std::pair<double, double>& begin, const std::pair<double, double>& end, double x) {
	auto dfdx = slope(begin, end);
	auto dx = x - begin.first;
	return {x, begin.second + dfdx * dx};
}

double slope(const std::pair<double, double>& begin, const std::pair<double, double>& end) {
	if(begin.first == end.first){
		return 0.0;
	}
	return (end.second - begin.second) / (end.first - begin.first);
}