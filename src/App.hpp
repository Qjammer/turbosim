#pragma once

#include <memory>
#include "Analysis/Infrastructure/ConvergenceAnalysisController.hpp"

class App {
		std::unique_ptr<ConvergenceAnalysisController> convergenceAnalysisController;
	public:
		App(
			std::unique_ptr<ConvergenceAnalysisController>&& convergenceAnalysisController
		)
			: convergenceAnalysisController(std::move(convergenceAnalysisController))
		{}


		int run(int argc, char** argv){
			std::vector<std::string> arguments(argv, argv+argc);

			//Extract controller path
			std::vector<std::string> controllerParams(arguments.begin()+1, arguments.end());
			std::string controllerPath = controllerParams[0];

			// Forward to controller
			if(controllerPath == "convergenceAnalysis"){
				return (*this->convergenceAnalysisController)(controllerParams);
			} else {
				throw std::runtime_error("Controller not found");
			}

		}
};