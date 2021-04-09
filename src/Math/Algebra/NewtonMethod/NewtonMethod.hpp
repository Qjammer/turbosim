#pragma once
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "../Jacobian/JacobianCalculator.hpp"
#include "Math/Parameter/ParameterRegister.hpp"
#include "Math/Constraint/ConstraintRegister.hpp"

class NewtonMethod {
		JacobianCalculator jacobianCalculator;
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		float relaxationFactor;
	public:
		NewtonMethod(
			JacobianCalculator jacobianCalculator,
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			:jacobianCalculator(jacobianCalculator)
			,parameterRegister(parameterRegister)
			,constraintRegister(constraintRegister)
			,relaxationFactor(0.5)
		{
		}

		void iterate(){
			const auto constraintValues = this->jacobianCalculator.getConstraintValues(this->constraintRegister->getAllConstraints());
			Eigen::SparseMatrix<double> constraintJacobian = this->jacobianCalculator.getJacobian(this->constraintRegister->getAllConstraints());
			Eigen::MatrixXd denseJacobian = constraintJacobian.toDense();
			Eigen::SparseQR<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<Eigen::SparseMatrix<double>::StorageIndex>> solver;
			solver.setPivotThreshold(1.0e-20);
			solver.compute(constraintJacobian);
			if(solver.info() != Eigen::Success){
				std::cerr<< "decomposition failed\n";
				return;
			}
			Eigen::VectorXd parameterDeltas = solver.solve(constraintValues);
			Eigen::VectorXd parameterDeltas2 = denseJacobian.fullPivHouseholderQr().solve(constraintValues);

			if(solver.info() != Eigen::Success){
				std::cerr<< "solving failed\n";
				return;
			}

			/*
			std::cerr<<"SJ\n"<<Eigen::MatrixXd(constraintJacobian)<<'\n';
			std::cerr<<"PD\n"<<Eigen::MatrixXd(parameterDeltas)<<'\n';
			std::cerr<<"PD2\n"<<Eigen::MatrixXd(parameterDeltas2)<<'\n';
			*/

			const auto parameterValues = this->getParameterValues(this->parameterRegister->getParameterMap());
			//const auto updatedParameterValues = this->getUpdatedParameterValues(parameterValues, parameterDeltas);
			const auto updatedParameterValues = this->getUpdatedParameterValues(parameterValues, parameterDeltas2);
			this->parameterRegister->updateValues(updatedParameterValues);
		}


		std::map<ParameterId, double> getParameterValues(const std::map<ParameterId, std::weak_ptr<Parameter>>& parameters) const
		{
			std::vector<Eigen::Triplet<double>> triplets;
			std::map<ParameterId, double> values;
			for(const auto& parameterPair: parameters){
				const auto& parameterWP = parameterPair.second;
				if(const auto& lp = parameterWP.lock()){
					const auto& parameterId = parameterPair.first;
					values[parameterId] = lp->getValue();
				}
			}

			return values;
		}

		std::map<ParameterId, double> getUpdatedParameterValues(const std::map<ParameterId, double>& parameterValues, const Eigen::VectorXd& deltas) const
		{
			std::map<ParameterId, double> newVals;
			for(const auto& parameterPack: parameterValues){
				const auto& parameterId = parameterPack.first;
				if(parameterId - 1 < deltas.rows()){
					const auto& paramDelta = deltas[parameterId - 1];
					newVals[parameterId] = parameterPack.second -  this->relaxationFactor * paramDelta;
				}
			}
			return newVals;
		}

		void setRelaxationFactor(float relaxationFactor) {
			this->relaxationFactor = relaxationFactor;
		}
};