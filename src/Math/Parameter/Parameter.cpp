#include "Parameter.hpp"

Parameter::Parameter(int id, float initialValue)
	:id(id)
	,value(initialValue)
{
}

int Parameter::getId() const
{
	return this->id;
}

float Parameter::getValue() const
{
	return this->value;
}

void Parameter::setValue(float value)
{
	this->value = value;
}