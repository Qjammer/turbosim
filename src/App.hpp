#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Analysis/ConvergenceAnalysis/Infrastructure/ConvergenceAnalysisController.hpp"
#include "Analysis/OperatingLineAnalysis/Infrastructure/OperatingLineAnalysisController.hpp"
#include "Analysis/IterateUntilConverged/Infrastructure/IterateUntilConvergedController.hpp"

class App {
		std::unique_ptr<ConvergenceAnalysisController> convergenceAnalysisController;
		std::unique_ptr<OperatingLineAnalysisController> operatingLineAnalysisController;
		std::unique_ptr<IterateUntilConvergedController> iterateUntilConvergedController;
	public:
		App(
			std::unique_ptr<ConvergenceAnalysisController>&& convergenceAnalysisController,
			std::unique_ptr<OperatingLineAnalysisController>&& operatingLineAnalysisController,
			std::unique_ptr<IterateUntilConvergedController>&& iterateUntilConvergedController
		)
			: convergenceAnalysisController(std::move(convergenceAnalysisController))
			, operatingLineAnalysisController(std::move(operatingLineAnalysisController))
			, iterateUntilConvergedController(std::move(iterateUntilConvergedController))
		{}


		int run(int argc, char** argv){
			std::vector<std::string> arguments(argv, argv+argc);

			//Extract controller path
			std::vector<std::string> controllerArguments(arguments.begin()+1, arguments.end());
			std::string controllerPath = controllerArguments[0];

			// Forward to controller
			if(controllerPath == "convergenceAnalysis"){
				return (*this->convergenceAnalysisController)(controllerArguments);
			} else if (controllerPath == "operatingLineAnalysis") {
				return (*this->operatingLineAnalysisController)(controllerArguments);
			} else if (controllerPath == "iterateUntilConverged") {
				return (*this->iterateUntilConvergedController)(controllerArguments);
			} else {
				throw std::runtime_error("Controller not found");
			}

		}
};