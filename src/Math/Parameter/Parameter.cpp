#include "Parameter.hpp"

Parameter::Parameter(int id, double initialValue, std::string units, std::string name)
	:id(id)
	,value(initialValue)
	,units(units)
	,name(name)
	,enabled(true)
{
}

ParameterId Parameter::getId() const
{
	return this->id;
}

double Parameter::getValue() const
{
	return this->value;
}

double Parameter::getDerivative() const {
	return this->enabled ? 1.0 : 0.0;
}

void Parameter::setValue(double value)
{
	this->value = value;
}

void Parameter::enable(bool enabled)
{
	this->enabled = enabled;
}
bool Parameter::isEnabled() const {
	return this->enabled;
}

Parameter& Parameter::operator=(double newValue){
	this->value = newValue;
	return *this;
}

const std::string& Parameter::getUnits() const{
	return this->units;
}

const std::string& Parameter::getName() const{
	return this->name;
}