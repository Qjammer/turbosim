#pragma once
#include "App.hpp"

class Kernel {

	public:
		Kernel (){}
	
	std::unique_ptr<App> build() const {

		auto parameterRegister = std::make_shared<ParameterRegister>();
		auto constraintRegister = std::make_shared<ConstraintRegister>();
		auto componentRegister = std::make_shared<ComponentRegister>();
		JacobianCalculator JC{};
		auto NM = std::make_shared<NewtonMethod>(JC,parameterRegister, constraintRegister);
		auto turbineFactory = std::make_shared<TurbineFactory>(parameterRegister, constraintRegister, componentRegister);
		auto tankFactory = std::make_shared<TankFactory>(parameterRegister, constraintRegister, componentRegister);
		auto combustorFactory = std::make_shared<CombustorFactory>(parameterRegister, constraintRegister, componentRegister);
		auto fluidBoundaryFactory = std::make_shared<FluidBoundaryFactory>(parameterRegister, constraintRegister, componentRegister);
		auto axialBoundaryFactory = std::make_shared<AxialBoundaryFactory>(parameterRegister, constraintRegister, componentRegister);

		return std::make_unique<App>(
			parameterRegister,
			constraintRegister,
			componentRegister,
			NM,
			turbineFactory,
			tankFactory,
			combustorFactory,
			fluidBoundaryFactory,
			axialBoundaryFactory
		);
	}
};
