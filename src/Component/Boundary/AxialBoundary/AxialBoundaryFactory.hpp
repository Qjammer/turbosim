#pragma once
#include "../../../Math/Parameter/ParameterRegister.hpp"
#include "../../../Math/Constraint/ConstraintRegister.hpp"
#include "AxialBoundary.hpp"

class AxialBoundaryFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ComponentRegister> componentRegister;
	public:
		AxialBoundaryFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ComponentRegister> componentRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
			, componentRegister(componentRegister)
		{}

		std::shared_ptr<AxialBoundary> build(
			ComponentId id,
			double initialP,
			double initialVelocity
		) {
			auto pP = parameterRegister->buildParameter(initialP, "W", "P_" + std::to_string(id));
			auto pw = parameterRegister->buildParameter(initialVelocity, "rad/s", "w_" + std::to_string(id));
			auto boundary = std::make_shared<AxialBoundary>(id,pw,pP);
			this->componentRegister->registerComponent(boundary);
			return boundary;
		}
};
