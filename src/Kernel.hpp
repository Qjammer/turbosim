#pragma once
#include "App.hpp"
#include "Component/ComponentRegister.hpp"
#include "Component/Turbine/TurbineFactory.hpp"
#include "Component/Combustor/CombustorFactory.hpp"
#include "Component/Boundary/FluidBoundary/FluidBoundaryFactory.hpp"
#include "Component/Boundary/AxialBoundary/AxialBoundaryFactory.hpp"
#include "Component/Tank/TankFactory.hpp"
#include "Setups/SingleSpoolSetup.hpp"


class Kernel {

	public:
		Kernel (){}
	
	std::unique_ptr<App> build() const {

		//DOMAIN
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

		//APPLICATION
		auto singleSpoolSetup = std::make_shared<SingleSpoolSetup>(
			tankFactory,
			turbineFactory,
			combustorFactory,
			fluidBoundaryFactory,
			axialBoundaryFactory,
			parameterRegister
		);
		auto convergenceAnalysis = std::make_shared<ConvergenceAnalysis>(NM, constraintRegister, parameterRegister);

		//INFRASTRUCTURE
		auto convergenceAnalysisController = std::make_unique<ConvergenceAnalysisController>(
			singleSpoolSetup,
			convergenceAnalysis
		);

		return std::make_unique<App>(
			std::move(convergenceAnalysisController)
		);
	}
};
