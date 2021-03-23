#pragma once
#include "../../Math/Parameter/ParameterRegister.hpp"
#include "../../Math/Constraint/ConstraintRegister.hpp"
#include "Turbine.hpp"
#include "FilePerformanceMap.hpp"

class TurbineFactory
{
		std::shared_ptr<ParameterRegister> parameterRegister;
		std::shared_ptr<ConstraintRegister> constraintRegister;
	public:
		TurbineFactory(
			std::shared_ptr<ParameterRegister> parameterRegister,
			std::shared_ptr<ConstraintRegister> constraintRegister
		)
			: parameterRegister(parameterRegister)
			, constraintRegister(constraintRegister)
		{}
		std::shared_ptr<Turbine> build(
			ComponentId id,
			const std::string& efficiency_file = "",
			const std::string& pi_file = ""
		) {
			auto performanceMap = std::make_unique<FilePerformanceMap>(efficiency_file, pi_file);
			auto turbine = std::make_shared<Turbine>(id, this->constraintRegister->getNextIds<4>(), std::move(performanceMap));
			this->constraintRegister->registerComponent(*turbine);
			return turbine;
		}
};
