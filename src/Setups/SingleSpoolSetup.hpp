#pragma once
#include "../Math/Parameter/ParameterRegister.hpp"
#include "../Component/Turbine/TurbineFactory.hpp"
#include "../Component/Boundary/AxialBoundary/AxialBoundaryFactory.hpp"
#include "../Component/Boundary/FluidBoundary/FluidBoundaryFactory.hpp"
#include "../Component/Tank/TankFactory.hpp"
#include "../Component/Combustor/CombustorFactory.hpp"

class SingleSpoolSetup {
		std::shared_ptr<TankFactory> tankFactory;
		std::shared_ptr<TurbineFactory> turbineFactory;
		std::shared_ptr<CombustorFactory> combustorFactory;
		std::shared_ptr<FluidBoundaryFactory> fluidBoundaryFactory;
		std::shared_ptr<AxialBoundaryFactory> axialBoundaryFactory;
		std::shared_ptr<ParameterRegister> parameterRegister;
	public:
		SingleSpoolSetup(
			std::shared_ptr<TankFactory> tankFactory,
			std::shared_ptr<TurbineFactory> turbineFactory,
			std::shared_ptr<CombustorFactory> combustorFactory,
			std::shared_ptr<FluidBoundaryFactory> fluidBoundaryFactory,
			std::shared_ptr<AxialBoundaryFactory> axialBoundaryFactory,
			std::shared_ptr<ParameterRegister> parameterRegister
		)
			: tankFactory(tankFactory)
			, turbineFactory(turbineFactory)
			, combustorFactory(combustorFactory)
			, fluidBoundaryFactory(fluidBoundaryFactory)
			, axialBoundaryFactory(axialBoundaryFactory)
			, parameterRegister(parameterRegister)
		{}

		void setup() const {

			// COMPONENT INSTANTIATION
			auto tank_C = this->tankFactory->build(1, 1.01e5, 288);
			auto compressor = this->turbineFactory->build(2, "resources/perf_maps/compressor_eff.tsv", "resources/perf_maps/compressor_pi.tsv");
			auto combustor = this->combustorFactory->build(3, 0.0016);
			auto turbine = this->turbineFactory->build(4, "resources/perf_maps/turbine_eff.tsv", "resources/perf_maps/turbine_pi.tsv");
			auto tank_H = this->tankFactory->build(5, 1.013e5, 288);

			// BOUNDARY INSTANTIATION / WIRING
			auto fluidB_LP_C = this->fluidBoundaryFactory->build(6, 19, 1.00e5, 288, 8.17e-3);
			tank_C->registerFluidBoundary(fluidB_LP_C);
			compressor->registerFluidBoundary(fluidB_LP_C, 0);

			auto fluidB_HP_C = this->fluidBoundaryFactory->build(7, 22, 1.9e5, 462, 6.4e-3);
			compressor->registerFluidBoundary(fluidB_HP_C, 1);
			combustor->registerFluidBoundary(fluidB_HP_C, 0);

			auto axialBoundary = this->axialBoundaryFactory->build(8, 3.4e5, 8950);
			turbine->registerAxialBoundary(axialBoundary);
			compressor->registerAxialBoundary(axialBoundary);

			auto fluidB_HP_H = this->fluidBoundaryFactory->build(9, 80, 1.7e5, 800, 3.4e-3);
			combustor->registerFluidBoundary(fluidB_HP_H, 1);
			turbine->registerFluidBoundary(fluidB_HP_H, 0);

			auto fluidB_LP_H = this->fluidBoundaryFactory->build(10, 62, 1.0e5, 628, 5.8e-3);
			turbine->registerFluidBoundary(fluidB_LP_H, 1);
			tank_H->registerFluidBoundary(fluidB_LP_H);

			this->parameterRegister->get(2).lock()->enable(false);

			this->parameterRegister->loadFromFile("resources/setups/single_spool_setup_initial_params.tsv");
		}
};