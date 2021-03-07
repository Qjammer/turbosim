#include <iostream>
#include "Kernel.hpp"
#include "Tests.hpp"

int main(int, char**) {

	//tests();
	// APPLICATION INITIALIZATION
	auto app = Kernel().build();

	// COMPONENT INSTANTIATION
	auto tank_C = app->getTankFactory()->build(1, 1.01e5, 293);
	auto compressor = app->getTurbineFactory()->build(2);
	auto combustor = app->getCombustorFactory()->build(3, 1.0);
	auto turbine = app->getTurbineFactory()->build(3);
	//auto tank_H = app->getTankFactory()->build(4, 1.01e5, 293);

	// BOUNDARY INSTANTIATION / WIRING
	auto fluidB_LP_C = app->getFluidBoundaryFactory()->build(1, 30.0, 1.00e5, 292, 5.0);
	tank_C->registerFluidBoundary(fluidB_LP_C);
	compressor->registerFluidBoundary(fluidB_LP_C, 0);

	auto fluidB_HP_C = app->getFluidBoundaryFactory()->build(2, 37.0, 7.80e5, 550, 1.0);
	compressor->registerFluidBoundary(fluidB_HP_C, 1);
	combustor->registerFluidBoundary(fluidB_HP_C, 0);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(3, -4.5e7, 0.0);
	compressor->registerAxialBoundary(axialBoundary, 0);
	turbine->registerAxialBoundary(axialBoundary, 0);

	auto fluidB_HP_H = app->getFluidBoundaryFactory()->build(4, 54.0, 7.80e5, 760, 1.0);
	combustor->registerFluidBoundary(fluidB_HP_H, 1);
	turbine->registerFluidBoundary(fluidB_HP_H, 0);

	auto fluidB_LP_H = app->getFluidBoundaryFactory()->build(4, 26.0, 2.20e5, 530, 5.0);
	turbine->registerFluidBoundary(fluidB_LP_H, 1);
	//tank_H->registerFluidBoundary(fluidB_LP_H);

	app->getParameterRegister()->get(2).lock()->enable(false);
	//app->getParameterRegister()->get(3).lock()->enable(false);
	//app->getParameterRegister()->get(7).lock()->enable(false);
	app->getParameterRegister()->get(15).lock()->enable(false);
	for(int i = 0; i < 1000; ++i){
		std::cout<<"it: "<<i<<'\n';
		std::cout<<"\tParameters:\n";
		app->getParameterRegister()->printParameterTable();
		std::cout<<"\tConstraints:\n";
		app->getConstraintRegister()->printConstraintTable();

		double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
		if(constraintNorm < 0.01){
			break;
		}

		app->getNewtonMethod()->iterate();
	}

	return 0;
}
