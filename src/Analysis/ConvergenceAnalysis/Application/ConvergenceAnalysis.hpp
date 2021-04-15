#pragma once

#include "../../../Math/Algebra/NewtonMethod/NewtonMethod.hpp"
#include "../../../Math/Constraint/ConstraintRegister.hpp"
#include "../../../Math/Parameter/ParameterRegister.hpp"

class ConvergenceAnalysis {
		std::shared_ptr<NewtonMethod> newtonMethod;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ParameterRegister> parameterRegister;
	public:
		ConvergenceAnalysis(
			std::shared_ptr<NewtonMethod> newtonMethod,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ParameterRegister> parameterRegister
		)
			: newtonMethod(newtonMethod)
			, constraintRegister(constraintRegister)
			, parameterRegister(parameterRegister)
		{}

		void analyze(
			float relaxationFactor,
			float variationHalfWidth,
			uint testPoints,
			uint maxIterations,
			std::ostream* output_stream
		) const {
			this->newtonMethod->setRelaxationFactor(relaxationFactor);
			// Find nearby minimum
			double minNorm = std::numeric_limits<double>::infinity();
			std::map<ParameterId, double> minNormValues;
			unsigned int itWithMinNorm = 0;
			for(unsigned int i = 0; i < maxIterations; ++i) {
				double constraintNorm = this->constraintRegister->getConstraintVectorNorm();

				if(std::isnan(constraintNorm)){
					std::cerr<<"NAN"<<std::endl;
					return;
				}
				if(constraintNorm < minNorm){
					minNorm = constraintNorm;
					minNormValues = this->parameterRegister->getValues();
					itWithMinNorm = i;
				}

				// If no better solution is found within N iterations, break
				if(i - itWithMinNorm > 20) {
					break;
				}

				this->newtonMethod->iterate();
			}


			// For each parameter
			auto parameters = this->parameterRegister->getParameterMap();
			for(const auto& p: parameters) {
				const auto& lp = p.second.lock();
				if(!lp->isEnabled()){
					continue;
				}
				this->parameterRegister->updateValues(minNormValues);
				auto val = lp->getValue();

				// Vary parameter from 50% to 150%
				for(const auto frac: getTestPoints(testPoints, 1.0 - variationHalfWidth, 1.0 + variationHalfWidth)){

					this->parameterRegister->updateValues(minNormValues);
					lp->setValue(val * frac);
					double constraintNorm = std::numeric_limits<double>::infinity();
					unsigned int i = 0;
					for(i = 0; i < maxIterations; ++i) {
						constraintNorm = this->constraintRegister->getConstraintVectorNorm();

						if(std::isnan(constraintNorm)){
							break;
						}
						if(constraintNorm < minNorm * 10){
							break;
						}

						this->newtonMethod->iterate();
					}
					auto solution_drift = this->getSolutionDrift(this->parameterRegister->getValues(), minNormValues);
					*output_stream<<lp->getId()<<'\t'<<lp->getName()<<'\t'<<val*frac<<'\t'<<frac<<'\t'<<i<<'\t'<<constraintNorm<<'\t'<<solution_drift<<'\n';
				}
				*output_stream<< std::flush;

			}
		}

		std::vector<double> getTestPoints(unsigned int N, double minFrac, double maxFrac) const {
			std::vector<double> arr(N);
			for(unsigned int i = 0; i < N; ++i) {
				arr[i] = (minFrac + (maxFrac - minFrac) * (double(i) / (N-1)));
			}
			return arr;
		}

		double getSolutionDrift(const std::map<ParameterId, double>& params, const std::map<ParameterId, double>& reference) const {
			auto var = 0.0;
			for (const auto& pair: reference) {
				var += powf( (params.at(pair.first) / pair.second) - 1.0, 2);
			}
			return sqrt(var);
		}
};