#pragma once
#include <vector>
#include <memory>
#include "../Parameter/Parameter.hpp"

class Constraint {
		int id;
	public:
		Constraint(int id);
		int getId() const;
		virtual float getValue() const = 0;
		virtual std::vector<std::weak_ptr<Parameter>> getDependentParameters() const = 0;// Returns list of parameters which are guaranteed to not return a null derivative
		virtual float getValueDerivative(const Parameter& p) const = 0;
};