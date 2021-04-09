#pragma once
#include "../Application/OperatingLineAnalysis.hpp"
#include "../../../Setups/SingleSpoolSetup.hpp"

class OperatingLineAnalysisController {
		std::shared_ptr<SingleSpoolSetup> setup;
		std::shared_ptr<OperatingLineAnalysis> analysis;
	public:
		OperatingLineAnalysisController(
			std::shared_ptr<SingleSpoolSetup> setup,
			std::shared_ptr<OperatingLineAnalysis> analysis
		)
			: setup(setup)
			, analysis(analysis)
		{}

		int operator()(const std::vector<std::string>& argv){
			assert(argv.size() == 8);

			int parameterIdToChange = stod(argv[1]);
			float relaxationFactor = stof(argv[2]);
			float variationLowerBound = stof(argv[3]);
			float variationUpperBound = stof(argv[4]);
			uint testPoints = stod(argv[5]);
			uint maxIterations = stod(argv[6]);
			std::string output_filename = argv[7];

			this->setup->setup();

			std::ofstream output_stream(output_filename);

			this->analysis->analyze(
				parameterIdToChange,
				relaxationFactor,
				variationLowerBound,
				variationUpperBound,
				testPoints,
				maxIterations,
				&output_stream
			);

			return 0;
		}
};