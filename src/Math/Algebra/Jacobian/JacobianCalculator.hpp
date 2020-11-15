#pragma once
#include <map>
#include "../../Constraint/Constraint.hpp"
#include "../../Parameter/Parameter.hpp"

class JacobianCalculator {
	public:
		std::map<ConstraintId,std::map<ParameterId, float>> getJacobian(const std::vector<std::weak_ptr<Constraint>>& constraints) const;
		std::map<ParameterId, float> getConstraintGradient(const std::shared_ptr<Constraint>& constraint) const;
		std::map<ConstraintId, float> getConstraintValues(const std::vector<std::weak_ptr<Constraint>>& constraints) const;
		float getNormOfConstraintValues(const std::map<ConstraintId, float>& constraintValues) const;
};