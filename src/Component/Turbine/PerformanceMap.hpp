#pragma once

#include "Turbine.hpp"

class PerformanceMap
{
	public:
		PerformanceMap()
		{}

	double getEfficiency(const Turbine& turbine) const {
		return 1.0;
	}
	double getEfficiencyDerivative(const Turbine& turbine, const Parameter& p) const {
		return 0.0;
	}

	double getPressureRatio(const Turbine& turbine) const {
		return 10.0;
	}
	double getPressureRatioDerivative(const Turbine& turbine, const Parameter& p) const {
		return 0.0;
	}

};
