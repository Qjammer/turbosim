#pragma once

class JacobianCalculator {
	public:
		std::map<int,std::map<int, float>> getSparseJacobian(const std::vector<std::weak_ptr<const Constraint>>& constraints)
		{
			std::map<int, std::map<int, float>> jacobian;
			for(const auto& constraint: constraints)
			{
				if(auto lc = constraint.lock()){
					jacobian[lc->getId()] = this->getConstraintGradient(lc);
				}
			}

			return jacobian;

		}
		std::map<int, float> getConstraintGradient(const std::shared_ptr<const Constraint>& constraint)
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

		std::map<int, float> getConstraintValues(const std::vector<std::weak_ptr<const Constraint>>& constraints)
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

		float getNormOfConstraintValues(const std::map<int, float>& constraintValues)
		{
			float norm = 0;
			for(const auto& constraintValuePair: constraintValues)
			{
				norm += constraintValuePair.second;
			}
			return norm;
		}
};