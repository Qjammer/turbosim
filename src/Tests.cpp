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

	auto turbine = app->getTurbineFactory()->build(1, "resources/perf_maps/turbine_eff.tsv", "resources/perf_maps/turbine_pi.tsv");

	auto fluidBoundaryHP = app->getFluidBoundaryFactory()->build(1, -30, 1.6e5, 370, 6.4e-3);
	turbine->registerFluidBoundary(fluidBoundaryHP, 0);

	auto fluidBoundaryLP = app->getFluidBoundaryFactory()->build(2, 30, 1.01e5, 300, 8.17e-3);
	turbine->registerFluidBoundary(fluidBoundaryLP, 1);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(3,1.9e4, 10600);
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

	auto tankHP = app->getTankFactory()->build(1, 1.6e5, 372);
	auto tankLP = app->getTankFactory()->build(2, 1.013e5, 288);
	auto turbine = app->getTurbineFactory()->build(3, "resources/perf_maps/turbine_eff.tsv", "resources/perf_maps/turbine_pi.tsv");

	auto fluidBoundaryHP = app->getFluidBoundaryFactory()->build(1, 29, 1.6e5, 372, 6.4e-3);
	tankHP->registerFluidBoundary(fluidBoundaryHP);
	turbine->registerFluidBoundary(fluidBoundaryHP, 0);

	auto fluidBoundaryLP = app->getFluidBoundaryFactory()->build(2,  29, 1.01e5, 300, 8.17e-3);
	turbine->registerFluidBoundary(fluidBoundaryLP, 1);
	tankLP->registerFluidBoundary(fluidBoundaryLP);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(3, 1.9e4, 10400);
	turbine->registerAxialBoundary(axialBoundary, 0);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void test_compressor_with_tanks_converges() {
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto tankLP = app->getTankFactory()->build(1, 1.013e5, 288);
	auto tankHP = app->getTankFactory()->build(2, 2.5e5, 372);
	auto compressor = app->getTurbineFactory()->build(3, "resources/perf_maps/compressor_eff.tsv", "resources/perf_maps/compressor_pi.tsv");

	auto fluidBoundaryLP = app->getFluidBoundaryFactory()->build(1,  25, 1.01e5, 288, 8.17e-3);
	tankLP->registerFluidBoundary(fluidBoundaryLP);
	compressor->registerFluidBoundary(fluidBoundaryLP, 0);

	auto fluidBoundaryHP = app->getFluidBoundaryFactory()->build(2, 13.4, 2.5e5, 373, 6.4e-3);
	compressor->registerFluidBoundary(fluidBoundaryHP, 1);
	tankHP->registerFluidBoundary(fluidBoundaryHP);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(3, -1.7e4, 10800);
	compressor->registerAxialBoundary(axialBoundary, 0);

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

	// COMPONENT INSTANTIATION
	auto tank1_H = app->getTankFactory()->build(1, 1.6e5, 440);
	auto turbine = app->getTurbineFactory()->build(2, "resources/perf_maps/turbine_eff.tsv", "resources/perf_maps/turbine_pi.tsv");
	auto tank1_L = app->getTankFactory()->build(3, 1.01e5, 360);

	auto tank2_L = app->getTankFactory()->build(4, 1.01e5, 288);
	auto compressor = app->getTurbineFactory()->build(5, "resources/perf_maps/compressor_eff.tsv", "resources/perf_maps/compressor_pi.tsv");
	auto tank2_H = app->getTankFactory()->build(6, 1.3e5, 447);

	// COMPONENT WIRING
	auto fluidBoundary1_H = app->getFluidBoundaryFactory()->build(1, 61, 1.57e5, 471, 3.4e-3);
	tank1_H->registerFluidBoundary(fluidBoundary1_H);
	turbine->registerFluidBoundary(fluidBoundary1_H, 0);

	auto fluidBoundary1_L = app->getFluidBoundaryFactory()->build(2,  44, 1.01e5, 359, 5.8e-3);
	turbine->registerFluidBoundary(fluidBoundary1_L, 1);
	tank1_L->registerFluidBoundary(fluidBoundary1_L);

	auto fluidBoundary2_L = app->getFluidBoundaryFactory()->build(3,  27, 1.01e5, 288, 8.17e-3);
	tank2_L->registerFluidBoundary(fluidBoundary2_L);
	compressor->registerFluidBoundary(fluidBoundary2_L, 0);

	auto fluidBoundary2_H = app->getFluidBoundaryFactory()->build(4, 28, 1.30e5, 446, 6.4e-3);
	compressor->registerFluidBoundary(fluidBoundary2_H, 1);
	tank2_H->registerFluidBoundary(fluidBoundary2_H);

	auto axialBoundary = app->getAxialBoundaryFactory()->build(5, 3.4e4, 10800);
	turbine->registerAxialBoundary(axialBoundary, 0);
	compressor->registerAxialBoundary(axialBoundary, 0);

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

	app->getParameterRegister()->get(2).lock()->enable(false);// mdot_3
	app->getParameterRegister()->get(11).lock()->enable(false);// w_3
	app->getParameterRegister()->get(3).lock()->setValue(630);

	for(int i = 0; i < 100; ++i){
		app->getNewtonMethod()->iterate();
		if(i==50){
			app->getParameterRegister()->get(11).lock()->enable(true);// w_3
		}
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	std::cout<<"PASSED"<<std::endl;
}

void tests(){
	test_tank_converges_outwards_flow();
	test_tank_converges_inwards_flow();
	test_turbine_converges();
	test_combustor_converges();
	test_turbine_with_tanks_converges();
	test_compressor_with_tanks_converges();
	test_4_tanks_2_turbines_converges();
	test_single_spool_converges();
}