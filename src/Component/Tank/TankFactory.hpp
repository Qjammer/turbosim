#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "../ComponentRegister.hpp"
#include "Tank.hpp"

class TankFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ComponentRegister> componentRegister;
	public:
		TankFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ComponentRegister> componentRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
			, componentRegister(componentRegister)
		{}

		std::shared_ptr<Tank> build(
			ComponentId id,
			double Pt,
			double Tt
		) {

			auto pTt = parameterRegister->buildParameter(Tt, "K", "Tt_" + std::to_string(id));
			auto tank = std::make_shared<Tank>(id, constraintRegister->getNextIds<3>(), Pt, Tt, pTt);
			this->constraintRegister->registerComponent(*tank);
			this->componentRegister->registerComponent(tank);
			return tank;
		}
};
