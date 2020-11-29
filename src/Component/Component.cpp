#include "Component.hpp"

Component::Component(ComponentId id)
	:id(id)
{
}

ComponentId Component::getId() const
{
	return this->id;
}

std::vector<std::weak_ptr<Constraint>> Component::getConstraints() const {
	return std::vector<std::weak_ptr<Constraint>> (this->constraints.begin(), this->constraints.end());
}