#pragma once
#include <vector>
#include "../Component.hpp"
#include "../../Boundary/AxialBoundary/AxialBoundary.hpp"
#include "../../Math/Constraint/Constraint.hpp"

class TurbineIsentropicProcessConstraint;
class TurbineEnergyConstraint;
class TurbineMassConstraint;
class PerformanceMap;
class FluidBoundary;
class TurbineFactory;

class Turbine : public Component {
	friend TurbineFactory;
	friend TurbineEnergyConstraint;
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
			std::array<ConstraintId, 3> constraintIds,
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

#include "../../Boundary/FluidBoundary/FluidBoundary.hpp"
class TurbineMassConstraint : public Constraint {
	Turbine* turbine;
	public:
		TurbineMassConstraint(ConstraintId id, Turbine* turbine): Constraint(id), turbine(turbine)
		{}

		double getValue() const override {
			return this->turbine->getInletMassFlow() + this->turbine->getOutletMassFlow();
		}

		double getValueDerivative(const Parameter& parameter) const override {
			return this->turbine->getInletMassFlowDerivative(parameter) + this->turbine->getOutletMassFlowDerivative(parameter);
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->turbine->getDependentParameters();
		}

};

class TurbineEnergyConstraint: public Constraint {
		Turbine* turbine;
	public:
		TurbineEnergyConstraint(ConstraintId id, Turbine* turbine): Constraint(id), turbine(turbine)
		{}

		double getValue() const override {
			return this->getInletEnergy()
			     + this->getOutletEnergy()
				 + this->turbine->getAxialPower();

		}
		double getValueDerivative(const Parameter& parameter) const override {
			return this->getInletEnergyDerivative(parameter)
				 + this->getOutletEnergyDerivative(parameter)
				 + this->turbine->getAxialPowerDerivative(parameter);
		}

		double getInletEnergy() const {
			auto mdot = this->turbine->getInletMassFlow();
			auto h = this->turbine->getInletTotalEnthalpy();
			return mdot * h;
		}
		double getInletEnergyDerivative(const Parameter& p) const {
			auto mdot  = this->turbine->getInletMassFlow();
			auto mdotD = this->turbine->getInletMassFlowDerivative(p);
			auto h  = this->turbine->getInletTotalEnthalpy();
			auto hD = this->turbine->getInletTotalEnthalpyDerivative(p);
			return mdot * hD + mdotD * h;
		}

		double getOutletEnergy() const {
			auto mdot = this->turbine->getOutletMassFlow();
			auto h = this->turbine->getOutletTotalEnthalpy();
			return mdot * h;
		}
		double getOutletEnergyDerivative(const Parameter& p) const {
			auto mdot  = this->turbine->getOutletMassFlow();
			auto mdotD = this->turbine->getOutletMassFlowDerivative(p);
			auto h  = this->turbine->getOutletTotalEnthalpy();
			auto hD = this->turbine->getOutletTotalEnthalpyDerivative(p);
			return mdot * hD + mdotD * h;
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->turbine->getDependentParameters();
		}
};

class TurbineIsentropicProcessConstraint: public Constraint {
		Turbine* turbine;
	public:
		TurbineIsentropicProcessConstraint(ConstraintId id, Turbine* turbine): Constraint(id), turbine(turbine)
		{}

		double getValue() const override {
			auto tratio = this->turbine->getTRatio();
			auto Pratio = this->getExpPRatio();
			return tratio - Pratio;
		}

		double getValueDerivative(const Parameter& parameter) const override {
			auto tratioD = this->turbine->getTRatioDerivative(parameter);
			auto PratioD = this->getExpPRatioDerivative(parameter);
			return tratioD - PratioD;
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->turbine->getDependentParameters();
		}

		double getExpPRatio() const {
			double exp = (AIR_GAMMA - 1)/AIR_GAMMA;
			double PRatio = this->turbine->getPRatio();
			return powf(PRatio, exp);
		}

		double getExpPRatioDerivative(const Parameter& p) const {
			double exp = (AIR_GAMMA - 1)/AIR_GAMMA;
			double PRatio = this->turbine->getPRatio();
			double dPRatiodParam = this->turbine->getPRatioDerivative(p);
			return exp * powf(PRatio, exp - 1) * dPRatiodParam;
		}

};
