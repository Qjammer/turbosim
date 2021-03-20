#pragma once
#include <vector>
#include "../Component.hpp"
#include "../../Boundary/AxialBoundary/AxialBoundary.hpp"
#include "../../Math/Constraint/Constraint.hpp"

class PerformanceMap;
class FluidBoundary;
class TurbineFactory;
class FilePerformanceMap;

class Turbine : public Component {
	friend TurbineFactory;
	friend FilePerformanceMap;
	friend int main(int, char**);
		std::shared_ptr<FluidBoundary> inlet;
		bool inletDir;
		std::shared_ptr<FluidBoundary> outlet;
		bool outletDir;

		std::shared_ptr<AxialBoundary> fwdAxle;
		bool fwdAxleDir;

		std::unique_ptr<PerformanceMap> performanceMap;
	public:
		Turbine(
			ComponentId id,
			std::array<ConstraintId, 4> constraintIds,
			std::unique_ptr<PerformanceMap> performanceMap
		);

		bool registerFluidBoundary(std::shared_ptr<FluidBoundary> boundary, int localBoundary);

		bool registerAxialBoundary(std::shared_ptr<AxialBoundary> boundary, int localBoundary);

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const;

		double getTRatio() const;
		double getTRatioDerivative(const Parameter& parameter) const;

		double getPRatio() const;
		double getPRatioDerivative(const Parameter& parameter) const;

		double getInletMassFlow() const;
		double getInletMassFlowDerivative(const Parameter& p) const;

		double getOutletMassFlow() const;
		double getOutletMassFlowDerivative(const Parameter& p) const;


		double getInletTotalEnthalpy() const;
		double getInletTotalEnthalpyDerivative(const Parameter& p) const;

		double getOutletTotalEnthalpy() const;
		double getOutletTotalEnthalpyDerivative(const Parameter& p) const;

		double getAxialPower() const;
		double getAxialPowerDerivative(const Parameter& p) const;
};
