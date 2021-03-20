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
			ComponentId id
		) {
			auto performanceMap = std::make_unique<FilePerformanceMap>("resources/perf_maps/test.tsv", "resources/perf_maps/test.tsv");
			auto turbine = std::make_shared<Turbine>(id, this->constraintRegister->getNextIds<4>(), std::move(performanceMap));
			this->constraintRegister->registerComponent(*turbine);
			return turbine;
		}
};
