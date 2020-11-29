#pragma once
#include "../Boundary.hpp"
#include "../../Math/Parameter/Parameter.hpp"
#define AIR_GAMMA 1.4
#define AIR_CP 1000.0

class FluidBoundary: public Boundary {
		std::vector<int> parameterIds;
		std::shared_ptr<Parameter> velocity;
		std::shared_ptr<Parameter> P;
		std::shared_ptr<Parameter> T;
		double section;
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
			return (double)*this->velocity * (orientation ? 1.0 : -1.0);
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

		double getSection() const {
			return this->section;
		}

};