#pragma once

typedef int ParameterId;

class Parameter {
	ParameterId id;
	float value;
	public:
		Parameter(ParameterId id, float initialValue);
		ParameterId getId() const;
		float getValue() const;
		void setValue(float v);
};