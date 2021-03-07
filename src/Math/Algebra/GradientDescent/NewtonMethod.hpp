#pragma once
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "Math/Parameter/ParameterRegister.hpp"
#include "Math/Constraint/ConstraintRegister.hpp"

class NewtonMethod {
		JacobianCalculator jacobianCalculator;
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		NewtonMethod(
			JacobianCalculator jacobianCalculator,
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			:jacobianCalculator(jacobianCalculator)
			,parameterRegister(parameterRegister)
			,constraintRegister(constraintRegister)
		{
		}

		void iterate(){
			const auto constraintValues = this->jacobianCalculator.getConstraintValues(this->constraintRegister->getAllConstraints());
			Eigen::SparseMatrix<double> constraintJacobian = this->jacobianCalculator.getJacobian(this->constraintRegister->getAllConstraints());
			Eigen::MatrixXd denseJacobian = constraintJacobian.toDense();
			Eigen::SparseQR<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<Eigen::SparseMatrix<double>::StorageIndex>> solver;
			solver.setPivotThreshold(1.0e-10);
			solver.compute(constraintJacobian);
			if(solver.info() != Eigen::Success){
				std::cout<< "decomposition failed\n";
				return;
			}
			Eigen::VectorXd parameterDeltas = solver.solve(constraintValues);
			Eigen::VectorXd parameterDeltas2 = denseJacobian.fullPivHouseholderQr().solve(constraintValues);

			if(solver.info() != Eigen::Success){
				std::cout<< "solving failed\n";
				return;
			}

			/*
			std::cout<<"SJ\n"<<Eigen::MatrixXd(constraintJacobian)<<'\n';
			std::cout<<"PD\n"<<Eigen::MatrixXd(parameterDeltas)<<'\n';
			std::cout<<"PD2\n"<<Eigen::MatrixXd(parameterDeltas2)<<'\n';
			*/

			const auto parameterValues = this->getParameterValues(this->parameterRegister->getParameterMap());
			//const auto updatedParameterValues = this->getUpdatedParameterValues(parameterValues, parameterDeltas);
			const auto updatedParameterValues = this->getUpdatedParameterValues(parameterValues, parameterDeltas2);
			this->updateParams(this->parameterRegister->getParameterMap(), updatedParameterValues);
		}

		void updateParams(std::map<ParameterId, std::weak_ptr<Parameter>>& parameters, const std::map<ParameterId, double>& newVals) const
		{
			for(const auto& valPack: newVals){
				const auto& parameterId = valPack.first;
				const auto& parameterVal = valPack.second;
				if(const auto& param = parameters.find(parameterId); param != parameters.end()){
					if(const auto& lp = param->second.lock()){
						lp->setValue(parameterVal);
					}
				}
			}

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
					newVals[parameterId] = parameterPack.second - paramDelta;
				}
			}
			return newVals;
		}
};