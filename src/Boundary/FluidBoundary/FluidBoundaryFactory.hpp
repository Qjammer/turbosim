#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "FluidBoundary.hpp"

class FluidBoundaryFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		FluidBoundaryFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
		{}

		std::shared_ptr<FluidBoundary> build(
			int id,
			double initialVelocity,
			double initialP,
			double initialT,
			double section
		)
		{
			auto pv = this->parameterRegister->buildParameter(initialVelocity, "m/s", "v_" + std::to_string(id));
			auto pP = this->parameterRegister->buildParameter(initialP, "Pa", "P_" + std::to_string(id));
			auto pT = this->parameterRegister->buildParameter(initialT, "K", "T_" + std::to_string(id));
			return std::make_shared<FluidBoundary>(id,pv,pP,pT,section);
		}
};
