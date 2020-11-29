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
	public:
		Tank(ComponentId id,std::vector<ConstraintId> constraintIds,  std::shared_ptr<FluidBoundary> boundary, double Pt, double Tt):
			Component(id), Pt(Pt), Tt(Tt), boundary(boundary)
		{
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TankEnergyConstraint>(constraintIds[0],this)));
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TankIsentropicConstraint>(constraintIds[1],this)));
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters(){
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
			return AIR_CP * this->boundary->getTemperature();
		}
		double getStaticEnthalpyDerivative(const Parameter& parameter) const {
			return AIR_CP * this->boundary->getTemperatureDerivative(parameter);
		}

		double getStagnationEnthalpy() const {
			return AIR_CP * this->Tt;
		}

		double getStagnationEnthalpyDerivative(const Parameter& parameter) const {
			return 0;
		}

		double getKineticEnergy() const {
			return powf(this->boundary->getVelocity(true), 2)/(2*AIR_CP);
		}

		double getKineticEnergyDerivative(const Parameter& parameter) const {
			auto V = this->boundary->getVelocity(true);
			auto dVdParam = this->boundary->getVelocityDerivative(parameter, true);

			return 2 * V * dVdParam / (2*AIR_CP);
		}

};

class TankEnergyConstraint : public Constraint {
		Tank* tank; // Constraint is owned by Tank, lifetime is guaranteed
	public:
		TankEnergyConstraint(ConstraintId id, Tank* tank): Constraint(id), tank(tank){}

		double getValue() const override {
			auto stagnationEnthalpy = this->tank->getStagnationEnthalpy();
			auto staticEnthalpy = this->tank->getStaticEnthalpy();
			auto kineticEnergy = this->tank->getKineticEnergy();

			return stagnationEnthalpy - (staticEnthalpy + kineticEnergy);
		}

		double getValueDerivative(const Parameter& parameter) const override {
			auto stagnationEnthalpyD = this->tank->getStagnationEnthalpyDerivative(parameter);
			auto staticEnthalpyD = this->tank->getStaticEnthalpyDerivative(parameter);
			auto kineticEnergyD = this->tank->getKineticEnergyDerivative(parameter);

			return stagnationEnthalpyD - (staticEnthalpyD + kineticEnergyD);
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