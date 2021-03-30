#pragma once
#include "../../../Math/Parameter/ParameterRegister.hpp"
#include "../../../Math/Constraint/ConstraintRegister.hpp"
#include "../../ComponentRegister.hpp"
#include "FluidBoundary.hpp"

class FluidBoundaryFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ComponentRegister> componentRegister;
	public:
		FluidBoundaryFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ComponentRegister> componentRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
			, componentRegister(componentRegister)
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
			auto boundary = std::make_shared<FluidBoundary>(id,pv,pP,pT,section);
			this->componentRegister->registerComponent(boundary);
			return boundary;
		}
};
