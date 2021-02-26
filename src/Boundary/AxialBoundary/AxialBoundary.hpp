#pragma once
#include "../Boundary.hpp"
#include "../../Math/Parameter/Parameter.hpp"

class AxialBoundary: public Boundary {
		std::vector<int> parameterIds;
		std::shared_ptr<Parameter> velocity;
		std::shared_ptr<Parameter> power;
		bool registered[2];// Keeps track if each endpoint is registered
	public:
		AxialBoundary(int id,
			std::shared_ptr<Parameter> velocity,
			std::shared_ptr<Parameter> power
		)
			:Boundary(id), velocity(velocity), power(power)
		{}

		double getVelocity(bool orientation) const {
			return this->velocity->getValue() * (orientation ? 1.0 : -1.0);
		}

		double getPower(bool orientation) const {
			return this->power->getValue() * (orientation ? 1.0 : -1.0);
		}

		std::vector<std::weak_ptr<Parameter>> getVelocityParameters() {
			return {this->velocity};
		}

		std::vector<std::weak_ptr<Parameter>> getPowerParameters() {
			return {this->power};
		}

		double getVelocityDerivative(const Parameter& param, bool orientation) const {
			if(param.getId() == this->velocity->getId()){
				return this->velocity->getDerivative() * (orientation ? 1.0 : -1.0);
			}
			return 0.0;
		}

		double getPowerDerivative(const Parameter& param, bool orientation) const {
			if(param.getId() == this->power->getId()){
				return this->power->getDerivative() * (orientation ? 1.0 : -1.0);
			}
			return 0.0;
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