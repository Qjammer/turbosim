#include <iostream>
#include "Kernel.hpp"

int main(int argc, char** argv) {
	auto kernel = Kernel();
	auto app = kernel.build();
	return app->run(argc, argv);
}
