#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "../ComponentRegister.hpp"
#include "Combustor.hpp"

class CombustorFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ComponentRegister> componentRegister;
	public:
		CombustorFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ComponentRegister> componentRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
			, componentRegister(componentRegister)
		{}
		std::shared_ptr<Combustor> build(
			ComponentId id,
			double mdot_f
		) {
			auto mdot_f_p = parameterRegister->buildParameter(mdot_f,"kg/s", "mdot_" + std::to_string(id));
			auto combustor = std::make_shared<Combustor>(id, this->constraintRegister->getNextIds<3>(), mdot_f_p);
			this->constraintRegister->registerComponent(*combustor);
			this->componentRegister->registerComponent(combustor);
			return combustor;
		}
};
