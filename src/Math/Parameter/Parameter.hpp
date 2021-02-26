#pragma once
#include <string>
typedef int ParameterId;

class Parameter {
	ParameterId id;
	double value;
	std::string units;
	std::string name;
	bool enabled;
	public:
		Parameter(ParameterId id, double initialValue, std::string units, std::string name);
		ParameterId getId() const;
		double getValue() const;
		double getDerivative() const;
		void setValue(double v);
		void enable(bool);
		Parameter& operator =(double newValue);
		const std::string& getUnits() const;
		const std::string& getName() const;
};