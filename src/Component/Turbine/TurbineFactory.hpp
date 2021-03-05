#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "Turbine.hpp"

class TurbineFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		TurbineFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
		{}
		std::shared_ptr<Turbine> build(
			ComponentId id
		) {
			auto turbine = std::make_shared<Turbine>(id, this->constraintRegister->getNextIds<3>());
			this->constraintRegister->registerComponent(*turbine);
			return turbine;
		}
};
