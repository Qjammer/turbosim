#pragma once

class Parameter {
	int id;
	float value;
	public:
		Parameter(int id, float initialValue);
		int getId() const;
		float getValue() const;
		void setValue(float v);
};