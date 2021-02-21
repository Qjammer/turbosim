#pragma once
#include "../Boundary.hpp"
#include "../../Math/Parameter/Parameter.hpp"
#define AIR_GAMMA 1.4
#define AIR_CP 1000.0
#define AIR_R ( AIR_CP * (1 - 1 / AIR_GAMMA) )

class FluidBoundary: public Boundary {
		std::vector<int> parameterIds;
		std::shared_ptr<Parameter> velocity;
		std::shared_ptr<Parameter> P;
		std::shared_ptr<Parameter> T;
		double section;
		bool registered[2];// Keeps track if each endpoint is registered
	public:
		FluidBoundary(int id,
			std::shared_ptr<Parameter> velocity,
			std::shared_ptr<Parameter> P,
			std::shared_ptr<Parameter> T,
			double section
		)
			:Boundary(id), velocity(velocity), P(P), T(T), section(section)
		{}

		double getVelocity(bool orientation) const {
			return double(*this->velocity) * (orientation ? 1.0 : -1.0);
		}


		double getTemperature() const {
			return *this->T;
		}

		double getPressure() const {
			return *this->P;
		}

		std::vector<std::weak_ptr<Parameter>> getVelocityParameters() {
			return {this->velocity};
		}

		std::vector<std::weak_ptr<Parameter>> getTemperatureParameters() {
			return {this->T};
		}

		std::vector<std::weak_ptr<Parameter>> getPressureParameters() {
			return {this->P};
		}

		double getVelocityDerivative(const Parameter& param, bool orientation) const {
			if(param.getId() == this->velocity->getId()){
				return orientation ? 1.0 : -1.0;
			}
			return 0.0;
		}

		double getPressureDerivative(const Parameter& param) const {
			if(param.getId() == this->P->getId()){
				return 1.0;
			}
			return 0.0;
		}

		double getTemperatureDerivative(const Parameter& param) const {
			if (param.getId() == this->T->getId()){
				return 1.0;
			}
			return 0.0;
		}

		double getSpecificKineticEnergy(bool orientation) const {
			return powf(this->getVelocity(orientation), 2) / 2;
		}

		double getSpecificKineticEnergyDerivative(const Parameter& param, bool orientation) const {
			auto v = this->getVelocity(orientation);
			auto dvdParam = this->getVelocityDerivative(param, orientation);
			return 2 * v * dvdParam / 2;
		}

		double getSpecificThermalInternalEnergy() const {
			return AIR_CP * this->getTemperature();
		}
		double getSpecificThermalInternalEnergyDerivative(const Parameter& param) const {
			return AIR_CP * this->getTemperatureDerivative(param);
		}

		double getSpecificTotalInternalEnergy(bool orientation) const {
			return this->getSpecificThermalInternalEnergy() + this->getSpecificKineticEnergy(orientation);
		}
		double getSpecificTotalInternalEnergyDerivative(const Parameter& param, bool orientation) const {
			return this->getSpecificThermalInternalEnergyDerivative(param) + this->getSpecificKineticEnergyDerivative(param, orientation);
		}
		
		double getSpecificPotentialWork() const {
			return AIR_R * this->getTemperature();
		}
		double getSpecificPotentialWorkDerivative(const Parameter& p) const {
			return AIR_R * this->getTemperatureDerivative(p);
		}

		double getSpecificEnthalpy(bool orientation) const {
			return this->getSpecificTotalInternalEnergy(orientation) + this->getSpecificPotentialWork();
		}

		double getSpecificEnthalpyDerivative(const Parameter& p, bool orientation) const {
			return this->getSpecificTotalInternalEnergyDerivative(p, orientation) + this->getSpecificPotentialWorkDerivative(p);
		}
		

		double getMassFlow(bool orientation) const
		{
			auto P = this->getPressure();
			auto u = this->getVelocity(orientation);
			auto A = this->getSection();
			auto T = this->getTemperature();

			return P*u*A/(T * AIR_R);
		}

		double getMassFlowDerivative(const Parameter& p, bool orientation) const
		{
			auto P = this->getPressure();
			auto dPdx = this->getPressureDerivative(p);
			auto u = this->getVelocity(orientation);
			auto dudx = this->getVelocityDerivative(p, orientation);
			auto A = this->getSection();
			auto T = this->getTemperature();
			auto dTdx = this->getTemperatureDerivative(p);

			auto num = P * u * A;
			auto dnumdx = ( P * dudx + dPdx * u ) * A;
			auto denom = 1 / (T *  AIR_R );
			auto ddenomdx = - ( dTdx / T ) * denom;

			return num * ddenomdx + dnumdx * denom;
		}

		double getSection() const {
			return this->section;
		}

		// Returns a tuple indicating whether the registration was successful, and which direction was registered
		std::tuple<bool, bool> registerEndpoint() {
			if(!this->registered[0]){
				this->registered[0] = true;
				return {true, true};
			}
			if(!this->registered[1]){
				this->registered[1] = true;
				return {true, false};
			}
			return {false, false};
		}
};