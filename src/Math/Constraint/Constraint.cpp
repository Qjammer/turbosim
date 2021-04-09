#include "Constraint.hpp"
#include "../Parameter/Parameter.hpp"

Constraint::Constraint(int id)
	:id(id)
{
}

int Constraint::getId() const {
	return this->id;
}