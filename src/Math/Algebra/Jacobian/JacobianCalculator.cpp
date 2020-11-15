#include "./JacobianCalculator.hpp"

std::map<ConstraintId,std::map<ParameterId, float>> JacobianCalculator::getJacobian(const std::vector<std::weak_ptr<Constraint>>& constraints) const
{
	std::map<ConstraintId, std::map<ParameterId, float>> jacobian;
	for(const auto& constraint: constraints)
	{
		if(auto lc = constraint.lock()){
			jacobian[lc->getId()] = this->getConstraintGradient(lc);
		}
	}

	return jacobian;

}

std::map<ParameterId, float> JacobianCalculator::getConstraintGradient(const std::shared_ptr<Constraint>& constraint) const
{
	std::map<int, float> gradient;
	for(const auto& parameter: constraint->getDependentParameters())
	{
		if(auto lp = parameter.lock()){
			gradient[lp->getId()] = constraint->getValueDerivative(*lp);
		}
	}
	return gradient;
}

std::map<ConstraintId, float> JacobianCalculator::getConstraintValues(const std::vector<std::weak_ptr<Constraint>>& constraints) const
{
	std::map<int, float> constraintValues;
	for(const auto& constraint: constraints)
	{
		if(auto lc = constraint.lock()){
			constraintValues[lc->getId()] = lc->getValue();
		}
	}

	return constraintValues;
}

float JacobianCalculator::getNormOfConstraintValues(const std::map<ConstraintId, float>& constraintValues) const
{
	float norm = 0;
	for(const auto& constraintValuePair: constraintValues)
	{
		norm += constraintValuePair.second;
	}
	return norm;
}