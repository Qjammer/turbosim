#pragma once
#include <vector>
#include "../Component.hpp"
#include "../../Boundary/FluidBoundary/FluidBoundary.hpp"

class Tank: public Component, std::enable_shared_from_this<Tank> {
		float Pt;
		float Tt;
		FluidBoundary boundary0;
	public:
		Tank(int id, FluidBoundary&& boundary, float Pt, float Tt):
			Component(id), Pt(Pt), Tt(Tt), boundary0(boundary)
		{}

		virtual int getBoundaryCount() const {
			return 1;
		};
		virtual const Boundary& getBoundary([[maybe_unused]] int i) const {
			return this->getBoundary(i);
		};
		virtual std::vector<std::weak_ptr<Boundary>> getBoundaries() {
			return std::vector<std::weak_ptr<Boundary>> {
				std::shared_ptr<Boundary>(this->shared_from_this(), this->boundary0)
			};
		}
		virtual int getInvariantCount() const {
			return 2;
		};
		virtual float getInvariant(int invariant) const {
			switch(invariant){
				case 0:
					return this->getEnergyInvariant();
				case 1:
					return this->getIsentropicInvariant();
				default:
					return 0;
			}
		};

		virtual float getInvariantDerivative(int invariant, int parameter) const {
			switch(invariant){
				case 0:
					return this->getEnergyInvariantDerivative(parameter);
				case 1:
					return this->getIsentropicInvariantDerivative(parameter);
				default:
					return 0;
			}
		};

		float getEnergyInvariant() const {
			auto Pratio = this->getPRatio();
			auto ke = this->getKineticEnergy();
			return Pratio - (1 + ke);
		}

		float getEnergyInvariantDerivative(int parameter) const {
			auto PratioDerivative = this->getPRatioDerivative(parameter);
			auto keDerivative = this->getKineticEnergyDerivative(parameter);

			return PratioDerivative - keDerivative;
		}

		float getIsentropicInvariant() const {
			if (boundary0.getVelocity(true) <= 0){
				return 0;
			}
			auto tratio = this->getTRatio();
			auto Pratio = this->getPRatio();
			return tratio - Pratio;
		}



		float getIsentropicInvariantDerivative(int parameter) const {
			// TODO
			return 0;
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

		float getKineticEnergyDerivative(int parameter) const {
			return 2 * this->boundary0.getVelocity(true) / (2*AIR_CP) * this->boundary0.getVelocityDerivative(parameter, true);
		}
};