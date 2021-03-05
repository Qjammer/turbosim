#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "Tank.hpp"

class TankFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		TankFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
		{}
		std::shared_ptr<Tank> build(
			ComponentId id,
			double Pt,
			double Tt
		) {

			auto pTt = parameterRegister->buildParameter(Tt, "K", "Tt_" + std::to_string(id));
			auto tank = std::make_shared<Tank>(2, constraintRegister->getNextIds<3>(), Pt, Tt, pTt);
			this->constraintRegister->registerComponent(*tank);
			return tank;
		}
};
