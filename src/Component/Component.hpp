#pragma once
#include <vector>
#include <memory>
#include "../Math/Constraint/Constraint.hpp"

typedef int ComponentId;

class Component {
		ComponentId id;
	protected:
		std::vector<std::shared_ptr<Constraint>> constraints;
	public:
		Component(ComponentId id);
		virtual ComponentId getId() const;
		virtual std::vector<std::weak_ptr<Constraint>> getConstraints() const;
};