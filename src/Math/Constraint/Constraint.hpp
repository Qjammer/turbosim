#pragma once
#include <vector>
#include <memory>

class Parameter;

typedef int ConstraintId;

class Constraint {
		ConstraintId id;
	public:
		Constraint(ConstraintId id);
		ConstraintId getId() const;
		virtual double getValue() const = 0;
		virtual std::vector<std::weak_ptr<Parameter>> getDependentParameters() const = 0;// Returns list of parameters which are guaranteed to not return a null derivative
		virtual double getValueDerivative(const Parameter& p) const = 0;
};