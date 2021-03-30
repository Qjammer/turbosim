#pragma once

#include <memory>
#include "Math/Algebra/Jacobian/JacobianCalculator.hpp"
#include "Math/Algebra/GradientDescent/NewtonMethod.hpp"
#include "Math/Parameter/ParameterRegister.hpp"
#include "Math/Constraint/ConstraintRegister.hpp"
#include "Component/Boundary/FluidBoundary/FluidBoundaryFactory.hpp"
#include "Component/Boundary/AxialBoundary/AxialBoundaryFactory.hpp"
#include "Component/Tank/TankFactory.hpp"
#include "Component/Turbine/TurbineFactory.hpp"
#include "Component/Combustor/CombustorFactory.hpp"

class App {
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ComponentRegister> componentRegister;
		std::shared_ptr<NewtonMethod> newtonMethod;
		std::shared_ptr<TurbineFactory> turbineFactory;
		std::shared_ptr<TankFactory> tankFactory;
		std::shared_ptr<CombustorFactory> combustorFactory;
		std::shared_ptr<FluidBoundaryFactory> fluidBoundaryFactory;
		std::shared_ptr<AxialBoundaryFactory> axialBoundaryFactory;
	public:
		App(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ComponentRegister> componentRegister,
			std::shared_ptr<NewtonMethod> newtonMethod,
			std::shared_ptr<TurbineFactory> turbineFactory,
			std::shared_ptr<TankFactory> tankFactory,
			std::shared_ptr<CombustorFactory> combustorFactory,
			std::shared_ptr<FluidBoundaryFactory> fluidBoundaryFactory,
			std::shared_ptr<AxialBoundaryFactory> axialBoundaryFactory
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
			, componentRegister(componentRegister)
			, newtonMethod(newtonMethod)
			, turbineFactory(turbineFactory)
			, tankFactory(tankFactory)
			, combustorFactory(combustorFactory)
			, fluidBoundaryFactory(fluidBoundaryFactory)
			, axialBoundaryFactory(axialBoundaryFactory)
		{}

		std::shared_ptr<ParameterRegister> getParameterRegister() {
			return this->parameterRegister;
		}

		std::shared_ptr<ConstraintRegister> getConstraintRegister() {
			return this->constraintRegister;
		}

		std::shared_ptr<NewtonMethod> getNewtonMethod() {
			return this->newtonMethod;
		}

		std::shared_ptr<TurbineFactory> getTurbineFactory() {
			return this->turbineFactory;
		}

		std::shared_ptr<TankFactory> getTankFactory() {
			return this->tankFactory;
		}

		std::shared_ptr<CombustorFactory> getCombustorFactory() {
			return this->combustorFactory;
		}

		std::shared_ptr<FluidBoundaryFactory> getFluidBoundaryFactory() {
			return this->fluidBoundaryFactory;
		}

		std::shared_ptr<AxialBoundaryFactory> getAxialBoundaryFactory() {
			return this->axialBoundaryFactory;
		}
};