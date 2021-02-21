#pragma once
#include <vector>
#include "../Component.hpp"
#include "../../Boundary/FluidBoundary/FluidBoundary.hpp"
#include "../../Math/Constraint/Constraint.hpp"

class TankEnergyConstraint;
class TankIsentropicConstraint;

class Tank : public Component {
	friend TankEnergyConstraint;
	friend TankIsentropicConstraint;
		double Pt;
		double Tt;
		std::shared_ptr<FluidBoundary> boundary;
		bool boundaryDir;
	public:
		Tank(ComponentId id,std::vector<ConstraintId> constraintIds, double Pt, double Tt):
			Component(id), Pt(Pt), Tt(Tt), boundary(nullptr), boundaryDir(false)
		{
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TankEnergyConstraint>(constraintIds[0],this)));
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TankIsentropicConstraint>(constraintIds[1],this)));
		}

		// Returns whether registration was successful
		bool registerFluidBoundary(std::shared_ptr<FluidBoundary> boundary){
			auto registerPair = boundary->registerEndpoint();
			if(!std::get<0>(registerPair))
				return false;

			this->boundary = boundary;
			this->boundaryDir = std::get<1>(registerPair);
			return true;
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const{
			if(this->boundary == nullptr){
				return {};
			}
			auto vParams = this->boundary->getVelocityParameters();
			auto TParams = this->boundary->getTemperatureParameters();
			auto PParams = this->boundary->getPressureParameters();

			std::vector<std::weak_ptr<Parameter>> allParams;
			allParams.reserve(vParams.size() + TParams.size() + PParams.size());

			allParams.insert(allParams.end(), vParams.begin(), vParams.end());
			allParams.insert(allParams.end(), TParams.begin(), TParams.end());
			allParams.insert(allParams.end(), PParams.begin(), PParams.end());
			return allParams;
		}

		double getTRatio() const {
			return this->Tt/this->getBoundaryTemperature();
		}

		double getTRatioDerivative(const Parameter& parameter) const {
			auto Tt = this->Tt;
			auto T = this->getBoundaryTemperature();
			auto dTdParam = this->getBoundaryTemperatureDerivative(parameter);
			return (- Tt / powf(T,2)) * dTdParam;

		}
		double getPRatio() const {
			return this->Pt/this->getBoundaryPressure();
		}

		double getPRatioDerivative(const Parameter& parameter) const {
			auto Pt = this->Pt;
			auto P = this->getBoundaryPressure();
			auto dPdParam = this->getBoundaryPressureDerivative(parameter);
			return (- Pt / powf(P,2)) * dPdParam;
		}

		double getStaticEnthalpy() const {
			return AIR_CP * this->getBoundaryTemperature();
		}
		double getStaticEnthalpyDerivative(const Parameter& parameter) const {
			return AIR_CP * this->getBoundaryTemperatureDerivative(parameter);
		}

		double getStagnationInternalEnergy() const {
			return AIR_CP * this->Tt;
		}
		double getStagnationInternalEnergyDerivative([[maybe_unused]] const Parameter& parameter) const {
			return 0;
		}

		double getSpecificPotentialWork() const {
			return AIR_R * this->Tt;
		}
		double getSpecificPotentialWorkDerivative(const Parameter& p) const {
			return 0;
		}
		double getStagnationEnthalpy() const {
			return this->getStagnationInternalEnergy() + this->getSpecificPotentialWork();
		}
		double getStagnationEnthalpyDerivative(const Parameter& p) const {
			return this->getStagnationInternalEnergyDerivative(p) + this->getSpecificPotentialWorkDerivative(p);

		}

		double getKineticEnergy() const {
			return this->boundary->getSpecificKineticEnergy(this->boundaryDir);
		}

		double getKineticEnergyDerivative(const Parameter& parameter) const {
			return this->boundary->getSpecificKineticEnergyDerivative(parameter, this->boundaryDir);
		}

		double getBoundaryTemperature() const {
			if(this->boundary == nullptr){
				return this->Tt;
			}
			return this->boundary->getTemperature();
		}

		double getBoundaryTemperatureDerivative(const Parameter& parameter) const {
			if(this->boundary == nullptr){
				return 0.0;
			}
			return this->boundary->getTemperatureDerivative(parameter);
		}

		double getBoundaryPressure() const {
			if(this->boundary == nullptr){
				return this->Pt;
			}
			return this->boundary->getPressure();
		}

		double getBoundaryPressureDerivative(const Parameter& parameter) const {
			if(this->boundary == nullptr){
				return 0.0;
			}
			return this->boundary->getPressureDerivative(parameter);
		}

		double getBoundaryVelocity() const {
			if(this->boundary == nullptr){
				return 0.0;
			}
			return this->boundary->getVelocity(this->boundaryDir);
		}

		double getBoundaryVelocityDerivative(const Parameter& parameter) const {
			if(this->boundary == nullptr){
				return 0.0;
			}
			return this->boundary->getVelocityDerivative(parameter, this->boundaryDir);
		}

		bool getFlowDirection() const {
			return this->getBoundaryVelocity() >= 0;
		}
};

class TankEnergyConstraint : public Constraint {
		Tank* tank; // Constraint is owned by Tank, lifetime is guaranteed
	public:
		TankEnergyConstraint(ConstraintId id, Tank* tank): Constraint(id), tank(tank){}

		double getValue() const override {
			auto stagnationEnthalpy = this->tank->getStagnationEnthalpy();
			auto boundaryEnthalpy = this->tank->boundary->getSpecificEnthalpy(this->tank->boundaryDir);

			auto energyDiff = stagnationEnthalpy - boundaryEnthalpy;
			return energyDiff;
		}

		double getValueDerivative(const Parameter& parameter) const override {

			auto stagnationEnthalpyD = this->tank->getStagnationEnthalpyDerivative(parameter);
			auto boundaryEnthalpyD = this->tank->boundary->getSpecificEnthalpyDerivative(parameter,this->tank->boundaryDir);

			auto energyDiffD = stagnationEnthalpyD - boundaryEnthalpyD;
			return energyDiffD;
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->tank->getDependentParameters();
		}
};

class TankIsentropicConstraint: public Constraint {
		Tank* tank;
	public:
		TankIsentropicConstraint(ConstraintId id, Tank* tank)
			:Constraint(id), tank(tank){}

		double getValue() const override {
				auto tratio = this->tank->getTRatio();
				auto Pratio = this->getExpPRatio();
				return tratio - Pratio;
			if(!tank->getFlowDirection()){// Inwards flow
				// If the flow is inwards, we assume that as long as the flow has enough energy to reach the tank pressure,
				// any leftover kinetic energy will be dissipated.
				return 0.0;
			} else {// Outwards flow
			}
		}

		double getValueDerivative(const Parameter& p) const override {
				auto tratio = this->tank->getTRatioDerivative(p);
				auto Pratio = this->getExpPRatioDerivative(p);
				return tratio - Pratio;
			if(!tank->getFlowDirection()){// Inwards flow
				return 0.0;
			} else {// Outwards flow
			}
		}

		double getExpPRatio() const {
			double exp = (AIR_GAMMA - 1)/AIR_GAMMA;
			double PRatio = this->tank->getPRatio();
			return powf(PRatio, exp);
		}

		double getExpPRatioDerivative(const Parameter& p) const {
			double exp = (AIR_GAMMA - 1)/AIR_GAMMA;
			double PRatio = this->tank->getPRatio();
			double dPRatiodParam = this->tank->getPRatioDerivative(p);
			return exp * powf(PRatio, exp - 1) * dPRatiodParam;
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->tank->getDependentParameters();
		}
};