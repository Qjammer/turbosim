#pragma once

class Turbine;
class Parameter;

class PerformanceMap
{
	public:
		virtual double getEfficiency(const Turbine& turbine) const = 0;
		virtual double getEfficiencyDerivative(const Turbine& turbine, const Parameter& p) const = 0;

		virtual double getPressureRatio(const Turbine& turbine) const = 0;
		virtual double getPressureRatioDerivative(const Turbine& turbine, const Parameter& p) const = 0;
};
