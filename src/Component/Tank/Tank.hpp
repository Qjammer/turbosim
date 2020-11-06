#pragma once
#include <
#include "../Component.hpp"
#include "../../Boundary/FluidBoundary/FluidBoundary.hpp"

class Tank: public Component{
		int id;
		float Pt;
		float Tt;
		FluidBoundary boundary0;
	public:
		virtual int getId() const {
			return this->id;
		};
		virtual int getBoundaries() const {
			return 1;
		};
		virtual Boundary& getBoundary(int i) const {
			return this->boundary0;
		};
		virtual int getInvariantCount() const {
			return 2;
		};
		virtual float getInvariant(int i) const {
			return 0;
			if(i > 1 || i < 0){
				//die
			}
			if(i == 0){
				return this->getEnergyInvariant();
			} else if(i == 1) {
				return this->getIsentropicInvariant();
			}
		};

		float getEnergyInvariant() const {
			auto Pratio = this->getPRatio();
			auto ke = this->getKineticEnergy();
			return Pratio - (1 + ke);
		}

		float getIsentropicInvariant() const {
			if (boundary0.getVelocity(true) <= 0){
				return 0;
			}
			auto tratio = this->getTRatio();
			auto Pratio = this->getPratio();
			return tratio - Pratio;
		}

		virtual float getInvariantDerivative(int invariant, int parameter) const {

		};
		float getEnergyInvariantDerivative(int parameter) const {
			Pratio = 
		}

		float getPRatio() const {
			return powf(this->Pt/this->boundary0.getPressure(), (AIR_GAMMA - 1) / AIR_GAMMA);
		}

		float getPRatioDerivative(int parameter) const {
			auto exp = (AIR_GAMMA - 1) / AIR_GAMMA;

			auto P = this->boundary0.getPressure();
			auto Pratio = this->getPRatio();
			auto dPdParam = this->boundary0.getPressureDerivative(parameter);
			return (- exp * Pratio / P) * dPdParam;

		}

		float getTRatio() const {
			return this->Tt/this->boundary0.getTemperature();
		}

		float getTRatioDerivative(int parameter) const {
			auto T = this->boundary0.getTemperature();
			auto TRatio = this->getTRatio();
			auto dTdParam = this->boundary0.getTemperatureDerivative(parameter);

			return (- TRatio / T )* dTdParam;
		}

		float getKineticEnergy() const {
			return powf(this->boundary0.getVelocity(true), 2)/(2*AIR_CP);
		}
};