#pragma once
#include <map>
#include "../../Constraint/Constraint.hpp"
#include "../../Parameter/Parameter.hpp"
#include <Eigen/Sparse>

class JacobianCalculator {
	public:
		Eigen::SparseMatrix<double> getJacobian(const std::map<ConstraintId, std::weak_ptr<Constraint>>& constraints) const;
		std::map<ParameterId, double> getConstraintGradient(const std::shared_ptr<Constraint>& constraint) const;
		Eigen::VectorXd getConstraintValues(const std::map<ConstraintId, std::weak_ptr<Constraint>>& constraints) const;
		double getNormOfConstraintValues(const Eigen::SparseMatrix<double>& constraintValues) const;
};