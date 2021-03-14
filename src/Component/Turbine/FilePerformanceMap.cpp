#include "FilePerformanceMap.hpp"

FilePerformanceMap::FilePerformanceMap(
	std::string_view filename
)
	: filename(filename)
{}

double FilePerformanceMap::getEfficiency(const Turbine& turbine) const {

}
double FilePerformanceMap::getEfficiencyDerivative(const Turbine& turbine, const Parameter& p) const {

}

double FilePerformanceMap::getPressureRatio(const Turbine& turbine) const {

}
double FilePerformanceMap::getPressureRatioDerivative(const Turbine& turbine, const Parameter& p) const {

}
