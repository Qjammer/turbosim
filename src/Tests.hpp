#pragma once
#include "Kernel.hpp"


void test_tank_equalizes_outwards_flow(){
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto tank = app->getTankFactory()->build(1,8.9e5,873);
	auto fluidBoundary = app->getFluidBoundaryFactory()->build(1, -10.0, 8.8e5, 872.95, 1.0);

	tank->registerFluidBoundary(fluidBoundary);

	for(int i = 0; i < 10; ++i){
		std::cout<<"it: "<<i<<'\n';
		std::cout<<"\tParameters:\n";
		app->getParameterRegister()->printParameterTable();
		std::cout<<"\tConstraints:\n";
		app->getConstraintRegister()->printConstraintTable();
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	assert(fluidBoundary->getVelocity(true) < 0.0);
}

void test_tank_equalizes_inwards_flow(){
	std::cout<<__FUNCTION__<<std::endl;
	auto app = Kernel().build();

	auto tank = app->getTankFactory()->build(1,8.9e5,873);
	auto fluidBoundary = app->getFluidBoundaryFactory()->build(1, 75.0, 8.7e5, 270, 1.0);

	tank->registerFluidBoundary(fluidBoundary);

	for(int i = 0; i < 10; ++i){
		std::cout<<"it: "<<i<<'\n';
		std::cout<<"\tParameters:\n";
		app->getParameterRegister()->printParameterTable();
		std::cout<<"\tConstraints:\n";
		app->getConstraintRegister()->printConstraintTable();
		app->getNewtonMethod()->iterate();
	}
	double constraintNorm = app->getConstraintRegister()->getConstraintVectorNorm();
	assert(constraintNorm < 0.1);
	assert(fluidBoundary->getVelocity(true) > 0.0);
}

void tests(){
	test_tank_equalizes_outwards_flow();
	test_tank_equalizes_inwards_flow();
}