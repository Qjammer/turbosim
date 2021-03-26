#include <iostream>
#include "Kernel.hpp"

void tests();

int main(int, char**) {

	//tests();
	// APPLICATION INITIALIZATION
	auto app = Kernel().build();

	// COMPONENT INSTANTIATION
	auto tank_C = app->getTankFactory()->build(1, 1.01e5, 288);
	auto compressor = app->getTurbineFactory()->build(2, "resources/perf_maps/compressor_eff.tsv", "resources/perf_maps/compressor_pi.tsv");
	auto combustor = app->getCombustorFactory()->build(3, 0.0016);
	auto turbine = app->getTurbineFactory()->build(4, "resources/perf_maps/turbine_eff.tsv", "resources/perf_maps/turbine_pi.tsv");
	auto tank_H = app->getTankFactory()->build(5, 1.013e5, 288);

	// BOUNDARY INSTANTIATION / WIRING
	auto fluidB_LP_C = app->getFluidBoundaryFactory()->build(1, 19, 1.00e5, 288, 8.17e-3);
	tank_C->registerFluidBoundary(fluidB_LP_C);
	compressor->registerFluidBoundary(fluidB_LP_C, 0);

	auto fluidB_HP_C = app->getFluidBoundaryFactory()->build(2, 22, 1.9e5, 462, 6.4e-3);
	compressor->registerFluidBoundary(fluidB_HP_C, 1);
	combustor->registerFluidBoundary(fluidB_HP_C, 0);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(3, 3.4e5, 8950);
	turbine->registerAxialBoundary(axialBoundary, 0);
	compressor->registerAxialBoundary(axialBoundary, 0);

	auto fluidB_HP_H = app->getFluidBoundaryFactory()->build(4, 80, 1.7e5, 800, 3.4e-3);
	combustor->registerFluidBoundary(fluidB_HP_H, 1);
	turbine->registerFluidBoundary(fluidB_HP_H, 0);

	auto fluidB_LP_H = app->getFluidBoundaryFactory()->build(5, 62, 1.0e5, 628, 5.8e-3);
	turbine->registerFluidBoundary(fluidB_LP_H, 1);
	tank_H->registerFluidBoundary(fluidB_LP_H);

	app->getParameterRegister()->get(2).lock()->enable(false);

	app->getParameterRegister()->loadFromFile("parameter_values_input.tsv");
	for(int i = 0; i < 400; ++i){
		std::cout<<"it: "<<i<<'\n';
		std::cout<<"\tParameters:\n";
		app->getParameterRegister()->printParameterTable();
		std::cout<<"\tConstraints:\n";
		app->getConstraintRegister()->printConstraintTable();

		double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
		if(std::isnan(constraintNorm)){
			std::cout<<"NAN"<<std::endl;
			break;
		}
		if(constraintNorm < 0.01){
			break;
		}

		app->getNewtonMethod()->iterate();
	}

	app->getParameterRegister()->saveToFile("parameter_values_output.tsv");

	return 0;
}
