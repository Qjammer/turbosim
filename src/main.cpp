#include <iostream>
#include "Kernel.hpp"
#include "Setups/SingleSpoolSetup.hpp"
#include "Analysis/ConvergenceAnalysis.hpp"

void tests();

int main(int argc, char** argv) {
	auto kernel = Kernel();
	auto app = kernel.build();
	return app->run(argc, argv);
}
