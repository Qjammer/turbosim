#include <iostream>
#include "Kernel.hpp"
#include "Tests.hpp"

int main(int, char**) {

	//tests();
	// APPLICATION INITIALIZATION
	auto app = Kernel().build();

	// BOUNDARY INSTANTIATION
	auto fluidBoundary1 = app->getFluidBoundaryFactory()->build(1, -10.0, 8.898e5, 872.95, 1.0);
	auto fluidBoundary2 = app->getFluidBoundaryFactory()->build(2,   8.9, 1.100e5, 380   , 5.0);
	auto axialBoundary1 = app->getAxialBoundaryFactory()->build(3,-1.4e7, 0.0);

	// COMPONENT INSTANTIATION
	auto tank1 = app->getTankFactory()->build(1,8.9e5, 873);

	// LP Tank
	auto pTtl = app->getParameterRegister()->buildParameter(480, "K", "Tt_o");
	auto tank2 = Tank(2, app->getConstraintRegister()->getNextIds<3>(), 0.9e5, 273, pTtl);
	app->getConstraintRegister()->registerComponent(tank2);

	auto turbine = app->getTurbineFactory()->build(3);

	// COMPONENT WIRING
	tank1->registerFluidBoundary(fluidBoundary1);
	tank2.registerFluidBoundary(fluidBoundary2);

	turbine->registerFluidBoundary(fluidBoundary1, 0);
	turbine->registerFluidBoundary(fluidBoundary2, 1);
	turbine->registerAxialBoundary(axialBoundary1, 0);


	for(int i = 0; i < 200; ++i){
		std::cout<<"it: "<<i<<'\n';
		std::cout<<"\tParameters:\n";
		app->getParameterRegister()->printParameterTable();
		std::cout<<"\tConstraints:\n";
		app->getConstraintRegister()->printConstraintTable();
		app->getNewtonMethod()->iterate();
	}

	return 0;
}
