#pragma once
#include <string>
#include "PerformanceMap.hpp"

class FilePerformanceMap: public PerformanceMap {
	std::string filename;
	public:
		FilePerformanceMap(
			std::string_view filename
		);

		virtual double getEfficiency(const Turbine& turbine) const override;
		virtual double getEfficiencyDerivative(const Turbine& turbine, const Parameter& p) const override;

		virtual double getPressureRatio(const Turbine& turbine) const override;
		virtual double getPressureRatioDerivative(const Turbine& turbine, const Parameter& p) const override;
};