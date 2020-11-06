#pragma once
#include "../Boundary.hpp"

class FluidBoundary: public Boundary{
		int id;
		float velocity;
		float P;
		float T;
	public:
		int getId() const {
			return this->id;
		}
		int getParametersCount() const {
			return 3;
		}
		float getParameter(int param) const {
			switch(param){
				case 0: 
					return this->velocity;
				case 1: 
					return this->P;
				case 2: 
					return this->T;
			}
			// Die
		}

		float getVelocity(bool orientation) const {
			return this->velocity * (orientation ? 1 : -1);
		}

		float getTemperature() const {
			return this->T;
		}

		float getVelocityDerivative(int param) const {
			if(param == 0){
				return 1.0;
			}
			return 0.0;
		}

		float getPressureDerivative(int param) const {
			if(param == 1){
				return 1.0;
			}
			return 0.0;
		}

		float getTemperatureDerivative(int param) const {
			if (param == 2){
				return 1.0;
			}
			return 0.0;
		}

		float getPressure() const {
			return this->P;
		}

}