#include <iostream>
#include "Kernel.hpp"
#include "Setups/SingleSpoolSetup.hpp"
#include "Analysis/ConvergenceAnalysis.hpp"

void tests();

int main(int, char**) {

	//tests();
	// APPLICATION INITIALIZATION
	auto kernel = Kernel();
	auto app = SingleSpoolSetup(kernel).setup();
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
