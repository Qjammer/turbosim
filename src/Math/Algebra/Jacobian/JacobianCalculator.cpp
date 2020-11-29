#include "./JacobianCalculator.hpp"
#include <math.h>

Eigen::SparseMatrix<double> JacobianCalculator::getJacobian(const std::vector<std::weak_ptr<Constraint>>& constraints) const
{
	std::map<ConstraintId, std::map<ParameterId, double>> jacobian;
	std::vector<Eigen::Triplet<double>> triplets;
	ConstraintId maxConstraintId = 0;
	ParameterId maxParameterId = 0;
	for(const auto& constraint: constraints)
	{
		if(auto lc = constraint.lock()){
			const auto constraintId = lc->getId();
			if(constraintId > maxConstraintId) {
				maxConstraintId = constraintId;
			}
			for(const auto& derivativeValuePair: this->getConstraintGradient(lc)){
				const auto parameterId = derivativeValuePair.first;
				if(parameterId > maxParameterId) {
					maxParameterId = parameterId;
				}
				const auto derivativeValue = derivativeValuePair.second;
				triplets.emplace_back(constraintId - 1, parameterId - 1, derivativeValue);
			}
		}
	}
	auto sparseMatrix = Eigen::SparseMatrix<double>(maxConstraintId, maxParameterId);
	sparseMatrix.setFromTriplets(triplets.begin(), triplets.end());
	sparseMatrix.makeCompressed();

	return sparseMatrix;
}

std::map<ParameterId, double> JacobianCalculator::getConstraintGradient(const std::shared_ptr<Constraint>& constraint) const
{
	std::map<int, double> gradient;
	for(const auto& parameter: constraint->getDependentParameters())
	{
		if(auto lp = parameter.lock()){
			gradient[lp->getId()] = constraint->getValueDerivative(*lp);
		}
	}
	return gradient;
}

Eigen::VectorXd JacobianCalculator::getConstraintValues(const std::vector<std::weak_ptr<Constraint>>& constraints) const
{
	std::map<int, double> constraintValues;
	std::vector<Eigen::Triplet<double>> triplets;
	ConstraintId maxConstraintId = 0;
	for(const auto& constraint: constraints)
	{
		if(auto lc = constraint.lock()){
			const auto constraintId = lc->getId();
			if(constraintId > maxConstraintId){
				maxConstraintId = constraintId;
			}
			triplets.emplace_back(constraintId - 1, 0, lc->getValue());
			constraintValues[lc->getId()] = lc->getValue();
		}
	}
	auto sparseMatrix = Eigen::SparseMatrix<double>(maxConstraintId, 1);
	sparseMatrix.setFromTriplets(triplets.begin(), triplets.end());

	return sparseMatrix;
}

double JacobianCalculator::getNormOfConstraintValues(const Eigen::SparseMatrix<double>& constraintValues) const
{
	return constraintValues.norm();
}