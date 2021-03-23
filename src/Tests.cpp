#include "Kernel.hpp"

void test_tank_converges_outwards_flow(){
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto tank = app->getTankFactory()->build(1, 9.0e5, 873);
	auto fluidBoundary = app->getFluidBoundaryFactory()->build(1, -10.0, 9.0e5, 873, 1.0);

	tank->registerFluidBoundary(fluidBoundary);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	assert(fluidBoundary->getVelocity(true) < 0.0);
	std::cout<<"PASSED"<<std::endl;
}

void test_tank_converges_inwards_flow(){
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto tank = app->getTankFactory()->build(1,9.0e5,873);
	auto fluidBoundary = app->getFluidBoundaryFactory()->build(1, 75.0, 9.0e5, 270, 1.0);

	tank->registerFluidBoundary(fluidBoundary);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	assert(fluidBoundary->getVelocity(true) > 0.0);
	std::cout<<"PASSED"<<std::endl;
}

void test_turbine_converges() {
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto fluidBoundaryHP = app->getFluidBoundaryFactory()->build(1, 10.0, 9.0e5, 873, 1.0);
	auto fluidBoundaryLP = app->getFluidBoundaryFactory()->build(2, -10.0, 9.0e5, 270, 5.0);
	auto axialBoundary = app->getAxialBoundaryFactory()->build(3,-1.4e7, 0.0);

	auto turbine = app->getTurbineFactory()->build(1);

	turbine->registerFluidBoundary(fluidBoundaryHP, 0);
	turbine->registerFluidBoundary(fluidBoundaryLP, 1);
	turbine->registerAxialBoundary(axialBoundary, 0);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void test_turbine_with_tanks_converges() {
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto fluidBoundaryHP = app->getFluidBoundaryFactory()->build(1, -10.0, 9.0e5, 873, 1.0);
	auto fluidBoundaryLP = app->getFluidBoundaryFactory()->build(2,  10.0, 1.01e5, 370, 5.0);
	auto axialBoundary = app->getAxialBoundaryFactory()->build(3,-1.4e7, 0.0);

	auto tankHP = app->getTankFactory()->build(1, 9.0e5, 873);
	auto tankLP = app->getTankFactory()->build(2, 1.01e5, 273);
	auto turbine = app->getTurbineFactory()->build(3);

	tankHP->registerFluidBoundary(fluidBoundaryHP);
	tankLP->registerFluidBoundary(fluidBoundaryLP);

	turbine->registerFluidBoundary(fluidBoundaryHP, 0);
	turbine->registerFluidBoundary(fluidBoundaryLP, 1);
	turbine->registerAxialBoundary(axialBoundary, 0);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void test_4_tanks_2_turbines_converges() {
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	// BOUNDARY INSTANTIATION
	auto fluidBoundary1_H = app->getFluidBoundaryFactory()->build(1, -10.0, 9.00e5, 873, 1.0);
	auto fluidBoundary1_L = app->getFluidBoundaryFactory()->build(2,  10.0, 1.01e5, 380, 5.0);
	auto fluidBoundary2_H = app->getFluidBoundaryFactory()->build(3, 10.0, 9.00e5, 873, 1.0);
	auto fluidBoundary2_L = app->getFluidBoundaryFactory()->build(4,  -10.0, 1.01e5, 380, 5.0);
	auto axialBoundary = app->getAxialBoundaryFactory()->build(5,-1.4e7, 0.0);

	// COMPONENT INSTANTIATION
	auto tank1_H = app->getTankFactory()->build(1, 9.0e5, 873);
	auto tank1_L = app->getTankFactory()->build(2, 1.01e5, 273);
	auto turbine1 = app->getTurbineFactory()->build(5);

	auto tank2_H = app->getTankFactory()->build(3, 9.0e5, 873);
	auto tank2_L = app->getTankFactory()->build(4, 1.01e5, 273);
	auto turbine2 = app->getTurbineFactory()->build(6);

	// COMPONENT WIRING
	tank1_H->registerFluidBoundary(fluidBoundary1_H);
	tank1_L->registerFluidBoundary(fluidBoundary1_L);
	tank2_H->registerFluidBoundary(fluidBoundary2_H);
	tank2_L->registerFluidBoundary(fluidBoundary2_L);

	turbine1->registerFluidBoundary(fluidBoundary1_H, 0);
	turbine1->registerFluidBoundary(fluidBoundary1_L, 1);
	turbine1->registerAxialBoundary(axialBoundary, 0);

	turbine2->registerFluidBoundary(fluidBoundary2_H, 0);
	turbine2->registerFluidBoundary(fluidBoundary2_L, 1);
	turbine2->registerAxialBoundary(axialBoundary, 0);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void test_combustor_converges() {
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	// BOUNDARY INSTANTIATION
	auto fluidBoundary_C = app->getFluidBoundaryFactory()->build(1, -10.0, 9.00e5, 373, 1.0);
	auto fluidBoundary_H = app->getFluidBoundaryFactory()->build(2,  10.0, 9.00e5, 573, 1.0);

	app->getParameterRegister()->get(1).lock()->enable(false);
	// COMPONENT INSTANTIATION
	auto combustor = app->getCombustorFactory()->build(1, 1.0);

	// COMPONENT WIRING
	combustor->registerFluidBoundary(fluidBoundary_C, 0);
	combustor->registerFluidBoundary(fluidBoundary_H, 1);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void test_single_spool_converges() {
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	// COMPONENT INSTANTIATION
	auto tank_C = app->getTankFactory()->build(1, 1.01e5, 288);
	auto compressor = app->getTurbineFactory()->build(2, "resources/perf_maps/compressor_eff.tsv", "resources/perf_maps/compressor_pi.tsv");
	auto combustor = app->getCombustorFactory()->build(3, 0.0002);
	auto turbine = app->getTurbineFactory()->build(4);
	auto tank_H = app->getTankFactory()->build(5, 1.013e5, 288);

	// BOUNDARY INSTANTIATION / WIRING
	auto fluidB_LP_C = app->getFluidBoundaryFactory()->build(1, 20, 1.008e5, 288, 8.17e-3);
	tank_C->registerFluidBoundary(fluidB_LP_C);
	compressor->registerFluidBoundary(fluidB_LP_C, 0);

	auto fluidB_HP_C = app->getFluidBoundaryFactory()->build(2, 13.5, 2.5e5, 373, 6.4e-3);
	compressor->registerFluidBoundary(fluidB_HP_C, 1);
	combustor->registerFluidBoundary(fluidB_HP_C, 0);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(3, 1.7e4, 0.0);
	turbine->registerAxialBoundary(axialBoundary, 0);
	compressor->registerAxialBoundary(axialBoundary, 0);

	auto fluidB_HP_H = app->getFluidBoundaryFactory()->build(4, 31, 2.25e5, 415, 3.4e-3);
	combustor->registerFluidBoundary(fluidB_HP_H, 1);
	turbine->registerFluidBoundary(fluidB_HP_H, 0);

	auto fluidB_LP_H = app->getFluidBoundaryFactory()->build(4, 33, 1.007e5, 408, 5.8e-3);
	turbine->registerFluidBoundary(fluidB_LP_H, 1);
	tank_H->registerFluidBoundary(fluidB_LP_H);

	app->getParameterRegister()->get(2).lock()->enable(false);// mdot_3
	app->getParameterRegister()->get(3).lock()->setValue(330);
	app->getParameterRegister()->get(8).lock()->enable(false);// P_2

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void tests(){
	test_tank_converges_outwards_flow();
	test_tank_converges_inwards_flow();
	test_turbine_converges();
	test_turbine_with_tanks_converges();
	test_4_tanks_2_turbines_converges();
	test_combustor_converges();
	test_single_spool_converges();
}