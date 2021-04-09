#pragma once
#include "../ConvergenceAnalysis.hpp"
#include "../../Setups/SingleSpoolSetup.hpp"

class ConvergenceAnalysisController {
		std::shared_ptr<SingleSpoolSetup> setup;
		std::shared_ptr<ConvergenceAnalysis> analysis;
	public:
		ConvergenceAnalysisController(
			std::shared_ptr<SingleSpoolSetup> setup,
			std::shared_ptr<ConvergenceAnalysis> analysis
		)
			: setup(setup)
			, analysis(analysis)
		{}

		int operator()(const std::vector<std::string>& argv){
			assert(argv.size() == 6);

			float relaxationFactor = stof(argv[1]);
			float variationHalfWidth = stof(argv[2]);
			double testPoints = stod(argv[3]);
			double maxIterations = stod(argv[4]);
			std::string output_filename = argv[5];

			this->setup->setup();

			std::ofstream output_stream(output_filename);

			this->analysis->analyze(
				relaxationFactor,
				variationHalfWidth,
				testPoints,
				maxIterations,
				&output_stream
			);

			return 0;
		}
};