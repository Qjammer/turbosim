#include "Parameter.hpp"

Parameter::Parameter(int id, double initialValue)
	:id(id)
	,value(initialValue)
{
}

int Parameter::getId() const
{
	return this->id;
}

double Parameter::getValue() const
{
	return this->value;
}

void Parameter::setValue(double value)
{
	this->value = value;
}

Parameter::operator double() const {
	return this->value;
}
Parameter& Parameter::operator=(double newValue){
	this->value = newValue;
	return *this;
}