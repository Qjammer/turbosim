#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "Combustor.hpp"

class CombustorFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		CombustorFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
		{}
		std::shared_ptr<Combustor> build(
			ComponentId id,
			double mdot_f
		) {
			auto mdot_f_p = parameterRegister->buildParameter(mdot_f,"kg/s", "mdot_" + std::to_string(id));
			auto combustor = std::make_shared<Combustor>(id, this->constraintRegister->getNextIds<3>(), mdot_f_p);
			this->constraintRegister->registerComponent(*combustor);
			return combustor;
		}
};
