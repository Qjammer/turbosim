#pragma once
#include "../Boundary.hpp"
#include "../../Math/Parameter/Parameter.hpp"
#define AIR_GAMMA 1.4
#define AIR_CP 1000.0
#define AIR_R ( AIR_CP * (1 - 1 / AIR_GAMMA) )

class AxialBoundary: public Boundary {
		std::vector<int> parameterIds;
		std::shared_ptr<Parameter> velocity;
		std::shared_ptr<Parameter> torque;
		bool registered[2];// Keeps track if each endpoint is registered
	public:
		AxialBoundary(int id,
			std::shared_ptr<Parameter> velocity,
			std::shared_ptr<Parameter> torque
		)
			:Boundary(id), velocity(velocity), torque(torque)
		{}

		double getVelocity(bool orientation) const {
			return double(*this->velocity) * (orientation ? 1.0 : -1.0);
		}

		double getTorque(bool orientation) const {
			return double(*this->velocity) * (orientation ? 1.0 : -1.0);
		}

		std::vector<std::weak_ptr<Parameter>> getVelocityParameters() {
			return {this->velocity};
		}

		std::vector<std::weak_ptr<Parameter>> getTorqueParameters() {
			return {this->torque};
		}

		double getVelocityDerivative(const Parameter& param, bool orientation) const {
			if(param.getId() == this->velocity->getId()){
				return orientation ? 1.0 : -1.0;
			}
			return 0.0;
		}

		double getTorqueDerivative(const Parameter& param) const {
			if(param.getId() == this->torque->getId()){
				return 1.0;
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