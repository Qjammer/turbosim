#pragma once
#include "../Boundary.hpp"
#include "../../Math/Parameter/Parameter.hpp"
#define AIR_GAMMA 1.4
#define AIR_CP 1000.0

class FluidBoundary: public Boundary{
		std::vector<int> parameterIds;
		Parameter velocity;
		Parameter P;
		Parameter T;
		float section;
	public:
		FluidBoundary(int id, Parameter&& velocity, Parameter&& P, Parameter&& T, float section):
			Boundary(id), velocity(velocity), P(P), T(T), section(section)
		{}


		float getVelocity(bool orientation) const {
			return this->velocity.getValue() * (orientation ? 1 : -1);
		}

		float getTemperature() const {
			return this->T.getValue();
		}

		float getVelocityDerivative(const Parameter& param, bool orientation) const {
			if(param.getId() == this->velocity.getId()){
				return orientation ? 1.0 : -1.0;
			}
			return 0.0;
		}

		float getPressureDerivative(const Parameter& param) const {
			if(param.getId() == this->P.getId()){
				return 1.0;
			}
			return 0.0;
		}

		float getTemperatureDerivative(const Parameter& param) const {
			if (param.getId() == this->T.getId()){
				return 1.0;
			}
			return 0.0;
		}

		float getPressure() const {
			return this->P.getValue();
		}

		float getSection() const {
			return this->section;
		}

};