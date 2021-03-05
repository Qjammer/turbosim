#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "AxialBoundary.hpp"

class AxialBoundaryFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		AxialBoundaryFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
		{}

		std::shared_ptr<AxialBoundary> build(
			int id,
			double initialP,
			double initialVelocity
		) {
			auto pP = parameterRegister->buildParameter(initialP, "W", "P_" + std::to_string(id));
			auto pw = parameterRegister->buildParameter(initialVelocity, "rad/s", "w_" + std::to_string(id));
			return std::make_shared<AxialBoundary>(id,pw,pP);
		}
};
