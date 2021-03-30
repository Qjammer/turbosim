#pragma once
#include <vector>
#include "../Component.hpp"
#include "../Boundary/AxialBoundary/AxialBoundary.hpp"
#include "../../Math/Constraint/Constraint.hpp"

#define FUEL_HEAT_ENERGY 43.1e6
#define FUEL_TEMPERATURE 273
#define PRODUCTS_FORMATION_ENTHALPY (AIR_CP * FUEL_TEMPERATURE)
#define COMBUSTOR_PRESSURE_DROP_RATIO 0.9
#define COMBUSTION_EFFICIENCY 0.9

class CombustorMassConstraint;
class CombustorEnergyConstraint;
class CombustorIsobaricProcessConstraint;

class Combustor : public Component {
	friend CombustorEnergyConstraint;
	friend CombustorIsobaricProcessConstraint;
		std::shared_ptr<FluidBoundary> inlet;
		bool inletDir;
		std::shared_ptr<FluidBoundary> outlet;
		bool outletDir;
		std::shared_ptr<Parameter> mdot_f;

	public:
		Combustor(
			ComponentId id, std::array<ConstraintId, 3> constraintIds, std::shared_ptr<Parameter> mdot_f
		): Component(id), mdot_f(mdot_f)
		{
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<CombustorMassConstraint>(constraintIds[0], this)));
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<CombustorEnergyConstraint>(constraintIds[1], this)));
			this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<CombustorIsobaricProcessConstraint>(constraintIds[2], this)));
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

			allParams.push_back(this->mdot_f);
			return allParams;
		}

		double getInletMassFlow() const {
			return this->inlet->getMassFlow(this->inletDir);
		}

		double getOutletMassFlow() const {
			return this->outlet->getMassFlow(this->outletDir);
		}

		double getFuelMassFlow() const {
			return - this->mdot_f->getValue();
		}
		double getFuelMassFlowDerivative(const Parameter& p) const {
			if(this->mdot_f->getId() != p.getId()){
				return 0.0;
			}
			return - this->mdot_f->getDerivative();
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

class CombustorMassConstraint : public Constraint {
	Combustor* combustor;
	public:
		CombustorMassConstraint(ConstraintId id, Combustor* combustor): Constraint(id), combustor(combustor)
		{}

		double getValue() const override {
			return this->combustor->getInletMassFlow()
			     + this->combustor->getOutletMassFlow()
				 + this->combustor->getFuelMassFlow();
		}

		double getValueDerivative(const Parameter& parameter) const override {
			return this->combustor->getInletMassFlowDerivative(parameter)
			     + this->combustor->getOutletMassFlowDerivative(parameter)
				 + this->combustor->getFuelMassFlowDerivative(parameter);
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->combustor->getDependentParameters();
		}

};

class CombustorEnergyConstraint: public Constraint {
		Combustor* combustor;
	public:
		CombustorEnergyConstraint(ConstraintId id, Combustor* combustor): Constraint(id), combustor(combustor)
		{}

		double getValue() const override {
			return this->getInletEnergy()
			     + this->getOutletEnergy()
				 + this->getFuelEnergy();

		}

		double getValueDerivative(const Parameter& parameter) const override {
			return this->getInletEnergyDerivative(parameter)
				 + this->getOutletEnergyDerivative(parameter)
				 + this->getFuelEnergyDerivative(parameter);
		}

		double getInletEnergy() const {
			auto mdot = this->combustor->getInletMassFlow();
			auto h = this->combustor->getInletTotalEnthalpy();
			return mdot * h;
		}
		double getInletEnergyDerivative(const Parameter& p) const {
			auto mdot  = this->combustor->getInletMassFlow();
			auto mdotD = this->combustor->getInletMassFlowDerivative(p);
			auto h  = this->combustor->getInletTotalEnthalpy();
			auto hD = this->combustor->getInletTotalEnthalpyDerivative(p);
			return mdot * hD + mdotD * h;
		}

		double getOutletEnergy() const {
			auto mdot = this->combustor->getOutletMassFlow();
			auto h = this->combustor->getOutletTotalEnthalpy();
			return mdot * h;
		}
		double getOutletEnergyDerivative(const Parameter& p) const {
			auto mdot  = this->combustor->getOutletMassFlow();
			auto mdotD = this->combustor->getOutletMassFlowDerivative(p);
			auto h  = this->combustor->getOutletTotalEnthalpy();
			auto hD = this->combustor->getOutletTotalEnthalpyDerivative(p);
			return mdot * hD + mdotD * h;
		}

		double getFuelEnergy() const {
			auto fuel_enthalpy = PRODUCTS_FORMATION_ENTHALPY * this->combustor->getFuelMassFlow();
			auto fuel_heat_combustion = FUEL_HEAT_ENERGY * this->combustor->getFuelMassFlow();
			return fuel_enthalpy + fuel_heat_combustion;

		}
		double getFuelEnergyDerivative(const Parameter& p) const {
			auto fuel_enthalpyD = PRODUCTS_FORMATION_ENTHALPY * this->combustor->getFuelMassFlowDerivative(p);
			auto fuel_heat_combustionD = FUEL_HEAT_ENERGY * this->combustor->getFuelMassFlowDerivative(p);
			return fuel_enthalpyD + fuel_heat_combustionD;
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->combustor->getDependentParameters();
		}
};

class CombustorIsobaricProcessConstraint: public Constraint {
		Combustor* combustor;
	public:
		CombustorIsobaricProcessConstraint(ConstraintId id, Combustor* combustor): Constraint(id), combustor(combustor)
		{}

		double getValue() const override {
			return COMBUSTOR_PRESSURE_DROP_RATIO * this->combustor->inlet->getPressure() - this->combustor->outlet->getPressure();
		}

		double getValueDerivative(const Parameter& parameter) const override {
			return COMBUSTOR_PRESSURE_DROP_RATIO * this->combustor->inlet->getPressureDerivative(parameter) - this->combustor->outlet->getPressureDerivative(parameter);
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->combustor->getDependentParameters();
		}
};
