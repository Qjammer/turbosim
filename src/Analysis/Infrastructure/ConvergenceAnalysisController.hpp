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
			/*
			float relaxationFactor = stof(argv[0]);
			float variationHalfWidth = stof(argv[1]);
			double testPoints = stod(argv[2]);
			double maxIterations = stod(argv[3]);
			*/
			//std::string output_filename = argv[4];

			this->setup->setup();

			std::string output_filename("resources/analysis_results/test2.tsv");
			std::ofstream output_stream(output_filename);

			this->analysis->analyze(
				0.5,
				0.5,
				21,
				200,
				&output_stream

			);

			return 0;
		}
};