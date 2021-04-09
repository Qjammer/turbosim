#pragma once
#include "../Application/IterateUntilConverged.hpp"
#include "../../../Setups/SingleSpoolSetup.hpp"

class IterateUntilConvergedController {
		std::shared_ptr<SingleSpoolSetup> setup;
		std::shared_ptr<IterateUntilConverged> analysis;
	public:
		IterateUntilConvergedController(
			std::shared_ptr<SingleSpoolSetup> setup,
			std::shared_ptr<IterateUntilConverged> analysis
		)
			: setup(setup)
			, analysis(analysis)
		{}

		int operator()(const std::vector<std::string>& argv){
			assert(argv.size() == 4);

			float relaxationFactor = stof(argv[1]);
			double maxIterations = stod(argv[2]);
			std::string output_filename = argv[3];

			this->setup->setup();

			this->analysis->analyze(
				relaxationFactor,
				maxIterations,
				output_filename
			);

			return 0;
		}
};