#pragma once
#include <vector>
#include "../Component.hpp"
#include "../../Boundary/AxialBoundary/AxialBoundary.hpp"
#include "../../Math/Constraint/Constraint.hpp"

class TurbineIsentropicProcessConstraint;
class TurbineEnergyConstraint;
class TurbineMassConstraint;

class Turbine : public Component {
	friend TurbineEnergyConstraint;
		std::shared_ptr<FluidBoundary> inlet;
		bool inletDir;
		std::shared_ptr<FluidBoundary> outlet;
		bool outletDir;

		std::shared_ptr<AxialBoundary> fwdAxle;
		bool fwdAxleDir;
	public:
		Turbine(
			ComponentId id, std::vector<ConstraintId> constraintIds
		): Component(id)
		{
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbineMassConstraint>(constraintIds[0], this)));
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbineEnergyConstraint>(constraintIds[1], this)));
			//this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbineIsentropicProcessConstraint>(constraintIds[2], this)));
		}

		bool registerFluidBoundary(std::shared_ptr<FluidBoundary> boundary, int localBoundary){
			auto registerPair = boundary->registerEndpoint();
			if(!std::get<0>(registerPair))
				return false;

			if(localBoundary == 0){
				this->inlet = boundary;
				this->inletDir = std::get<1>(registerPair);
			} else {
				this->outlet = boundary;
				this->outletDir = std::get<1>(registerPair);
			}
			return true;
		}

		bool registerAxialBoundary(std::shared_ptr<AxialBoundary> boundary, int localBoundary){
			auto registerPair = boundary->registerEndpoint();
			if(std::get<0>(registerPair)){
				return false;
			}
			if(localBoundary == 0){
				this->fwdAxle = boundary;
				this->fwdAxleDir = std::get<1>(registerPair);
			}
		}


		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const{
			std::vector<std::weak_ptr<Parameter>> allParams;
			if(this->inlet != nullptr){
				auto vParams = this->inlet->getVelocityParameters();
				auto TParams = this->inlet->getTemperatureParameters();
				auto PParams = this->inlet->getPressureParameters();
				allParams.insert(allParams.end(), vParams.begin(), vParams.end());
				allParams.insert(allParams.end(), TParams.begin(), TParams.end());
				allParams.insert(allParams.end(), PParams.begin(), PParams.end());
			}

			if(this->outlet != nullptr){
				auto vParams = this->outlet->getVelocityParameters();
				auto TParams = this->outlet->getTemperatureParameters();
				auto PParams = this->outlet->getPressureParameters();
				allParams.insert(allParams.end(), vParams.begin(), vParams.end());
				allParams.insert(allParams.end(), TParams.begin(), TParams.end());
				allParams.insert(allParams.end(), PParams.begin(), PParams.end());
			}

			return allParams;
		}

		double getTRatio() const {
			return this->inlet->getTemperature()/this->outlet->getTemperature();
		}

		double getTRatioDerivative(const Parameter& parameter) const {
			auto Ti = this->inlet->getTemperature();
			auto To = this->outlet->getTemperature();
			auto dTidx = this->inlet->getTemperatureDerivative(parameter);
			auto dTodx = this->outlet->getTemperatureDerivative(parameter);

			return (dTidx * To - dTodx * Ti) / (To * To);
		}

		double getPRatio() const {
			return this->inlet->getPressure()/this->outlet->getPressure();
		}

		double getPRatioDerivative(const Parameter& parameter) const {
			auto Pi = this->inlet->getPressure();
			auto Po = this->outlet->getPressure();
			auto dPidx = this->inlet->getPressureDerivative(parameter);
			auto dPodx = this->outlet->getPressureDerivative(parameter);

			return (dPidx * Po - dPodx * Pi) / (Po * Po);
		}

		double getInletMassFlow() const {
			return this->inlet->getMassFlow(this->inletDir);
		}

		double getOutletMassFlow() const {
			return this->outlet->getMassFlow(this->outletDir);
		}

		double getInletMassFlowDerivative(const Parameter& p) const {
			return this->inlet->getMassFlowDerivative(p, this->inletDir);
		}

		double getOutletMassFlowDerivative(const Parameter& p) const {
			return this->outlet->getMassFlowDerivative(p, this->outletDir);
		}
		
		double getInletTotalEnthalpy() {
			return this->inlet->getSpecificEnthalpy(this->inletDir);
		}
		double getInletTotalEnthalpyDerivative(const Parameter& p) {
			return this->inlet->getSpecificEnthalpyDerivative(p, this->inletDir);
		}

		double getOutletTotalEnthalpy() {
			return this->outlet->getSpecificEnthalpy(this->outletDir);
		}
		double getOutletTotalEnthalpyDerivative(const Parameter& p) {
			return this->outlet->getSpecificEnthalpyDerivative(p, this->outletDir);
		}


};

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
			return this->turbine->getInletTotalEnthalpy() - this->turbine->getOutletTotalEnthalpy();

		}

		double getValueDerivative(const Parameter& parameter) const override {
			return this->turbine->getInletTotalEnthalpyDerivative(parameter) - this->turbine->getOutletTotalEnthalpyDerivative(parameter);
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
			return 0;
			return tratio - Pratio;
		}

		double getValueDerivative(const Parameter& parameter) const override {
			auto tratio = this->turbine->getTRatioDerivative(parameter);
			auto Pratio = this->getExpPRatioDerivative(parameter);
			return 0;
			return tratio - Pratio;
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
