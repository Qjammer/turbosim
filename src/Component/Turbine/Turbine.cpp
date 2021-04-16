#include "Turbine.hpp"
#include "PerformanceMap.hpp"
#include "../Boundary/FluidBoundary/FluidBoundary.hpp"

#define MECH_EFFICIENCY 0.95

class TurbineIsentropicProcessConstraint;
class TurbineEnergyConstraint;
class TurbineMassConstraint;
class TurbinePerformanceConstraint;

Turbine::Turbine(
	ComponentId id,
	std::array<ConstraintId, 4> constraintIds,
	std::unique_ptr<PerformanceMap> performanceMap
)
	: Component(id)
	, performanceMap(std::move(performanceMap))
{
	this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbineMassConstraint>(constraintIds[0], this)));
	this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbineEnergyConstraint>(constraintIds[1], this)));
	this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbineIsentropicProcessConstraint>(constraintIds[2], this)));
	this->constraints.push_back(std::static_pointer_cast<Constraint>(std::make_shared<TurbinePerformanceConstraint>(constraintIds[3], this)));
}

bool Turbine::registerFluidBoundary(std::shared_ptr<FluidBoundary> boundary, int localBoundary){
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

bool Turbine::registerAxialBoundary(const std::shared_ptr<AxialBoundary>& boundary){
	auto registerPair = boundary->registerEndpoint();
	if(!std::get<0>(registerPair)){
		return false;
	}
	this->fwdAxle = boundary;
	this->fwdAxleDir = std::get<1>(registerPair);
	return true;
}


std::vector<std::weak_ptr<Parameter>> Turbine::getDependentParameters() const{
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

	if(this->fwdAxle != nullptr){
		auto wParams = this->fwdAxle->getVelocityParameters();
		auto PParams = this->fwdAxle->getPowerParameters();
		allParams.insert(allParams.end(), wParams.begin(), wParams.end());
		allParams.insert(allParams.end(), PParams.begin(), PParams.end());
	}

	return allParams;
}

double Turbine::getTRatio() const {
	return this->outlet->getTemperature()/this->inlet->getTemperature();
}

double Turbine::getTRatioDerivative(const Parameter& parameter) const {
	auto Ti = this->inlet->getTemperature();
	auto To = this->outlet->getTemperature();
	auto dTidx = this->inlet->getTemperatureDerivative(parameter);
	auto dTodx = this->outlet->getTemperatureDerivative(parameter);

	return (dTodx * Ti - To * dTidx) / (Ti * Ti);
}

double Turbine::getPRatio() const {
	return this->outlet->getPressure()/this->inlet->getPressure();
}

double Turbine::getPRatioDerivative(const Parameter& parameter) const {
	auto Pi = this->inlet->getPressure();
	auto Po = this->outlet->getPressure();
	auto dPidx = this->inlet->getPressureDerivative(parameter);
	auto dPodx = this->outlet->getPressureDerivative(parameter);
	return (dPodx * Pi - Po * dPidx ) / (Pi * Pi);
}

double Turbine::getInletMassFlow() const {
	return this->inlet->getMassFlow(this->inletDir);
}

double Turbine::getOutletMassFlow() const {
	return this->outlet->getMassFlow(this->outletDir);
}

double Turbine::getInletMassFlowDerivative(const Parameter& p) const {
	return this->inlet->getMassFlowDerivative(p, this->inletDir);
}

double Turbine::getOutletMassFlowDerivative(const Parameter& p) const {
	return this->outlet->getMassFlowDerivative(p, this->outletDir);
}

double Turbine::getInletTotalEnthalpy() const {
	return this->inlet->getSpecificEnthalpy(this->inletDir);
}
double Turbine::getInletTotalEnthalpyDerivative(const Parameter& p) const {
	return this->inlet->getSpecificEnthalpyDerivative(p, this->inletDir);
}

double Turbine::getOutletTotalEnthalpy() const {
	return this->outlet->getSpecificEnthalpy(this->outletDir);
}
double Turbine::getOutletTotalEnthalpyDerivative(const Parameter& p) const {
	return this->outlet->getSpecificEnthalpyDerivative(p, this->outletDir);
}

double Turbine::getAxialPower() const {
	return this->fwdAxle->getPower(this->fwdAxleDir);
}

double Turbine::getAxialPowerDerivative(const Parameter& p) const {
	return this->fwdAxle->getPowerDerivative(p,this->fwdAxleDir);
}

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
			auto fluidDiff = this->getInletEnergy() + this->getOutletEnergy();
			auto axialPower = this->turbine->getAxialPower();

			return fluidDiff +  axialPower * (axialPower > 0 ? 1/MECH_EFFICIENCY: MECH_EFFICIENCY);
		}
		double getValueDerivative(const Parameter& parameter) const override {
			auto fluidDiff = this->getInletEnergyDerivative(parameter) + this->getOutletEnergyDerivative(parameter);
			auto axialPower = this->turbine->getAxialPowerDerivative(parameter);

			return fluidDiff +  axialPower * (axialPower > 0 ? 1/MECH_EFFICIENCY: MECH_EFFICIENCY);
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
			auto isentropic_eff = this->turbine->performanceMap->getEfficiency(*this->turbine);
			if(this->turbine->getAxialPower() < 0) {
				return isentropic_eff * tratio - Pratio;
			} else {
				return tratio - isentropic_eff * Pratio;
			}
		}

		double getValueDerivative(const Parameter& parameter) const override {
			auto tratioD = this->turbine->getTRatioDerivative(parameter);
			auto PratioD = this->getExpPRatioDerivative(parameter);
			auto isentropic_eff = this->turbine->performanceMap->getEfficiency(*this->turbine);
			auto isentropic_eff_d = this->turbine->performanceMap->getEfficiencyDerivative(*this->turbine, parameter);
			if(this->turbine->getAxialPower() < 0) {
				auto tratio = this->turbine->getTRatio();
				return (isentropic_eff * tratioD + isentropic_eff_d * tratio) - PratioD;
			} else {
				auto Pratio = this->getExpPRatio();
				return tratioD - (isentropic_eff * PratioD + isentropic_eff_d * Pratio);
			}
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

class TurbinePerformanceConstraint: public Constraint {
		Turbine* turbine;
	public:
		TurbinePerformanceConstraint(ConstraintId id, Turbine* turbine): Constraint(id), turbine(turbine)
		{}

		double getValue() const override {
			auto pi = this->turbine->getPRatio();
			auto perf_pi = this->turbine->performanceMap->getPressureRatio(*this->turbine);
			if(this->turbine->getAxialPower() < 0) {
				return pi - perf_pi;
			} else {
				return pi - 1 / perf_pi;
			}
		}
		double getValueDerivative(const Parameter& parameter) const override {
			auto pi_d = this->turbine->getPRatioDerivative(parameter);
			auto perf_pi = this->turbine->performanceMap->getPressureRatio(*this->turbine);
			auto perf_pi_d = this->turbine->performanceMap->getPressureRatioDerivative(*this->turbine, parameter);
			if(this->turbine->getAxialPower() < 0) {
				return pi_d - perf_pi_d;
			} else {
				return pi_d + perf_pi_d / powf(perf_pi, 2);
			}
		}

		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const override {
			return this->turbine->getDependentParameters();
		}
};
