#pragma once
#include<array>

class ConvergenceAnalysis {
	public:
		template<unsigned int testPoints, unsigned int maxIterations>
		void analyze(App& app, float relaxationFactor) const {
			app.getNewtonMethod()->setRelaxationFactor(relaxationFactor);
			// Find nearby minimum
			double minNorm = std::numeric_limits<double>::infinity();
			std::map<ParameterId, double> minNormValues;
			unsigned int itWithMinNorm = 0;
			for(unsigned int i = 0; i < maxIterations; ++i) {
				double constraintNorm = app.getConstraintRegister()->getConstraintVectorNorm();

				if(std::isnan(constraintNorm)){
					std::cerr<<"NAN"<<std::endl;
					return;
				}
				if(constraintNorm < minNorm){
					minNorm = constraintNorm;
					minNormValues = app.getParameterRegister()->getValues();
					itWithMinNorm = i;
				}

				// If no better solution is found within N iterations, break
				if(i - itWithMinNorm > 20) {
					break;
				}

				app.getNewtonMethod()->iterate();
			}


			// For each parameter
			auto parameters = app.getParameterRegister()->getParameterMap();
			for(const auto& p: parameters) {
				const auto& lp = p.second.lock();
				if(!lp->isEnabled()){
					continue;
				}
				app.getParameterRegister()->updateValues(minNormValues);
				auto val = lp->getValue();

				// Vary parameter from 50% to 150%
				for(const auto frac: getTestPoints<testPoints>(0.5, 1.5)){

					app.getParameterRegister()->updateValues(minNormValues);
					lp->setValue(val * frac);
					double constraintNorm = std::numeric_limits<double>::infinity();
					unsigned int i = 0;
					for(i = 0; i < maxIterations; ++i) {
						constraintNorm = app.getConstraintRegister()->getConstraintVectorNorm();

						if(std::isnan(constraintNorm)){
							break;
						}
						if(constraintNorm < minNorm * 10){
							break;
						}

						app.getNewtonMethod()->iterate();
					}
					std::cout<<lp->getId()<<'\t'<<lp->getName()<<'\t'<<val*frac<<'\t'<<frac<<'\t'<<i<<'\t'<<constraintNorm<<'\n';
				}

			}
		}

		template<unsigned int N>
		std::array<double, N> getTestPoints(double minFrac, double maxFrac) const {
			std::array<double, N> arr;
			for(unsigned int i = 0; i < N; ++i) {
				arr[i] = (minFrac + (maxFrac - minFrac) * (double(i) / (N-1)));
			}
			return arr;
		}
};