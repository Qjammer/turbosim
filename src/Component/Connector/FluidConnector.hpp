#pragma once
#include <memory>
#include "../Component.hpp"
#include "../../Boundary/Boundary.hpp"
#include "../../Boundary/FluidBoundary/FluidBoundary.hpp"

class FluidConnector: public Component{
		std::weak_ptr<FluidBoundary> boundary0;
		std::weak_ptr<FluidBoundary> boundary1;
		public:
			FluidConnector(int id, const std::weak_ptr<FluidBoundary>& boundary0, const std::weak_ptr<FluidBoundary>& boundary1):
				Component(id), boundary0(boundary0), boundary1(boundary1)
			{
			}
			int getBoundaryCount() const override {
				return 2;
			}
			const Boundary& getBoundary(int i) const override {
				return this->getBoundary(i);
			}
			Boundary& getBoundary(int i) override {
				switch(i){
					case 0:
						return *this->boundary0.lock();
					case 1:
						return *this->boundary1.lock();
				}
			}

			int getInvariantCount() const override {
				return 3;
			}

			double getInvariant(int i) const override {
				switch(i) {
					case 0:
						return this->getMassInvariant();
					case 1:
						return this->getEnergyInvariant();
					default:
						return 0;
				}
			}

			double getInvariantDerivative(int invariant, int parameter) const override {
				switch(i) {
					case 0:
						return this->getMassInvariantDerivative(parameter);
					case 1:
						return this->getEnergyInvariantDerivative(parameter);
				}
			}

			double getMassInvariant() const {
				auto m0 = this->getMassFlow(0);
				auto m1 = this->getMassFlow(1);
				return m0 - m1;
			}

			double getEnergyInvariant() const {
				auto E0 = this->getEnergy(0);
				auto E1 = this->getEnergy(1);

				return E0 - E1;
			}


			double getMassFlow(int i) const {
				auto& boundary = (i == 0 ? this->boundary0. : this->boundary1).lock();
				auto v = boundary.lock()->getVelocity(false);
				auto A = boundary.lock()->getSection();
				auto P = boundary.lock()->getPressure();

				return v * A * powf(P, 1/AIR_GAMMA);
			}

			double getEnergy(int i) const
				auto& boundary = (i == 0 ? this->boundary0. : this->boundary1).lock();
				auto v = boundary.lock()->getVelocity(false);
				auto T = boundary.lock()->getTemperature();

				return fpow(v,2)/2 + AIR_CP * T;
			}


};