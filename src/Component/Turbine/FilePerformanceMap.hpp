#pragma once
#include <string>
#include <map>
#include "PerformanceMap.hpp"

class FilePerformanceMap: public PerformanceMap {
		void parseFile(std::ifstream& file, std::map<double, std::map<double, double>>& map);
		std::map<double, std::map<double, double>> efficiencyValues;
		std::map<double, std::map<double, double>> pressureRatioValues;
		void insertPoint(std::map<double, std::map<double, double>>& map, double x, double y, double v);
	public:
		FilePerformanceMap(
			std::string filename_efficiency, std::string filename_pi
		);

		virtual double getEfficiency(const Turbine& turbine) const override;
		virtual double getEfficiencyDerivative(const Turbine& turbine, const Parameter& p) const override;

		virtual double getPressureRatio(const Turbine& turbine) const override;
		virtual double getPressureRatioDerivative(const Turbine& turbine, const Parameter& p) const override;
};