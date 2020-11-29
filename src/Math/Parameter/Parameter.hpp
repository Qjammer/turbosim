#pragma once

typedef int ParameterId;

class Parameter {
	ParameterId id;
	double value;
	public:
		Parameter(ParameterId id, double initialValue);
		ParameterId getId() const;
		double getValue() const;
		void setValue(double v);
		operator double() const;
		Parameter& operator =(double newValue);
};