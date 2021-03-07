#pragma once
#include "App.hpp"

class Kernel {

	public:
		Kernel (){}
	
	std::unique_ptr<App> build() const {

		auto parameterRegister = std::make_shared<ParameterRegister>();
		auto constraintRegister = std::make_shared<ConstraintRegister>();
		JacobianCalculator JC{};
		auto NM = std::make_shared<NewtonMethod>(JC,parameterRegister, constraintRegister);
		auto turbineFactory = std::make_shared<TurbineFactory>(parameterRegister, constraintRegister);
		auto tankFactory = std::make_shared<TankFactory>(parameterRegister, constraintRegister);
		auto combustorFactory = std::make_shared<CombustorFactory>(parameterRegister, constraintRegister);
		auto fluidBoundaryFactory = std::make_shared<FluidBoundaryFactory>(parameterRegister, constraintRegister);
		auto axialBoundaryFactory = std::make_shared<AxialBoundaryFactory>(parameterRegister, constraintRegister);

		return std::make_unique<App>(
			parameterRegister,
			constraintRegister,
			NM,
			turbineFactory,
			tankFactory,
			combustorFactory,
			fluidBoundaryFactory,
			axialBoundaryFactory
		);
	}
};
