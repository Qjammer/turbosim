#pragma once
#include <vector>
#include <memory>
#include "../Parameter/Parameter.hpp"

typedef int ConstraintId;
class Constraint {
		ConstraintId id;
	public:
		Constraint(ConstraintId id);
		ConstraintId getId() const;
		virtual float getValue() const = 0;
		virtual std::vector<std::weak_ptr<Parameter>> getDependentParameters() const = 0;// Returns list of parameters which are guaranteed to not return a null derivative
		virtual float getValueDerivative(const Parameter& p) const = 0;
};