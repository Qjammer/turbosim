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
			return this->Tt/this->boundary->getTemperature();
		}

		double getTRatioDerivative(const Parameter& parameter) const {
			auto Tt = this->Tt;
			auto T = this->boundary->getTemperature();
			auto dTdParam = this->boundary->getTemperatureDerivative(parameter);
			return (- Tt / powf(T,2)) * dTdParam;

		}
		double getPRatio() const {
			return this->Pt/this->boundary->getPressure();
		}

		double getPRatioDerivative(const Parameter& parameter) const {
			auto Pt = this->Pt;
			auto P = this->boundary->getPressure();
			auto dPdParam = this->boundary->getPressureDerivative(parameter);
			return (- Pt / powf(P,2)) * dPdParam;
		}

		double getStaticEnthalpy() const {
			return this->boundary->getSpecificThermalInternalEnergy();
		}
		double getStaticEnthalpyDerivative(const Parameter& parameter) const {
			return this->boundary->getSpecificThermalInternalEnergyDerivative(parameter);
		}

		double getSpecificThermalInternalEnergy() const {
			return AIR_CV * this->Tt;
		}
		double getSpecificThermalInternalEnergyDerivative([[maybe_unused]] const Parameter& parameter) const {
			return 0;
		}

		double getSpecificPotentialWork() const {
			return AIR_R * this->Tt;
		}
		double getSpecificPotentialWorkDerivative(const Parameter& p) const {
			return 0;
		}

		double getSpecificTotalInternalEnergy() const {
			return this->getSpecificThermalInternalEnergy();
		}
		double getSpecificTotalInternalEnergyDerivative(const Parameter& p) const {
			return this->getSpecificThermalInternalEnergyDerivative(p);
		}

		double getSpecificEnthalpy() const {
			return this->getSpecificTotalInternalEnergy() + this->getSpecificPotentialWork();
		}
		double getSpecificEnthalpyDerivative(const Parameter& p) const {
			return this->getSpecificTotalInternalEnergyDerivative(p) + this->getSpecificPotentialWorkDerivative(p);

		}

		double getBoundarySpecificKineticEnergy() const {
			return this->boundary->getSpecificKineticEnergy(this->boundaryDir);
		}

		double getSpecificKineticEnergyDerivative(const Parameter& parameter) const {
			return this->boundary->getSpecificKineticEnergyDerivative(parameter, this->boundaryDir);
		}

		double getBoundaryVelocity() const {
			return this->boundary->getVelocity(this->boundaryDir);
		}

		double getBoundaryVelocityDerivative(const Parameter& parameter) const {
			return this->boundary->getVelocityDerivative(parameter, this->boundaryDir);
		}

};

class TankEnergyConstraint : public Constraint {
		Tank* tank; // Constraint is owned by Tank, lifetime is guaranteed
	public:
		TankEnergyConstraint(ConstraintId id, Tank* tank): Constraint(id), tank(tank){}

		double getValue() const override {
			auto tankEnthalpy = this->tank->getSpecificEnthalpy();
			auto boundaryEnthalpy = this->tank->boundary->getSpecificEnthalpy(this->tank->boundaryDir);

			return tankEnthalpy - boundaryEnthalpy;
		}

		double getValueDerivative(const Parameter& parameter) const override {

			auto tankEnthalpyD = this->tank->getSpecificEnthalpyDerivative(parameter);
			auto boundaryEnthalpyD = this->tank->boundary->getSpecificEnthalpyDerivative(parameter,this->tank->boundaryDir);

			return (tankEnthalpyD - boundaryEnthalpyD) * 1.5;
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
		}

		double getValueDerivative(const Parameter& p) const override {
				auto tratio = this->tank->getTRatioDerivative(p);
				auto Pratio = this->getExpPRatioDerivative(p);
				return tratio - Pratio;
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