#pragma once
#include "../../../Math/Algebra/NewtonMethod/NewtonMethod.hpp"
#include "../../../Math/Constraint/ConstraintRegister.hpp"
#include "../../../Math/Parameter/ParameterRegister.hpp"

class OperatingLineAnalysis {
		std::shared_ptr<NewtonMethod> newtonMethod;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ParameterRegister> parameterRegister;
	public:
		OperatingLineAnalysis(
			std::shared_ptr<NewtonMethod> newtonMethod,
			std::shared_ptr<ConstraintRegister> constraintRegister,
			std::shared_ptr<ParameterRegister> parameterRegister
		)
			: newtonMethod(newtonMethod)
			, constraintRegister(constraintRegister)
			, parameterRegister(parameterRegister)
		{}

		void analyze(
			ParameterId parameterIdToChange,
			float relaxationFactor,
			float variationLowerBound,
			float variationUpperBound,
			uint testPoints,
			uint maxIterations,
			std::ostream* output_stream
		) const {
			this->newtonMethod->setRelaxationFactor(relaxationFactor);
			// Find nearby minimum
			double startingMinNorm = std::numeric_limits<double>::infinity();
			std::map<ParameterId, double> minNormValues;
			unsigned int itWithMinNorm = 0;
			for(unsigned int i = 0; i < maxIterations; ++i) {
				double constraintNorm = this->constraintRegister->getConstraintVectorNorm();

				if(std::isnan(constraintNorm)){
					std::cerr<<"NAN"<<std::endl;
					return;
				}
				if(constraintNorm < startingMinNorm){
					startingMinNorm = constraintNorm;
					minNormValues = this->parameterRegister->getValues();
					itWithMinNorm = i;
				}

				// If no better solution is found within N iterations, break
				if(i - itWithMinNorm > 20) {
					break;
				}

				this->newtonMethod->iterate();
			}


			// For the given parameter
			auto parameter_p = this->parameterRegister->get(parameterIdToChange);
			const auto& lp = parameter_p.lock();

			this->printFileHeader(output_stream);
			auto val = lp->getValue();

			this->parameterRegister->updateValues(minNormValues);

			int firstPts = testPoints * (1.0 - variationLowerBound) / (variationUpperBound - variationLowerBound);
			for(const auto frac: getTestPoints(firstPts, 1.0, variationLowerBound)){
				uint itWithMinNorm = 0;
				double minNorm = std::numeric_limits<double>::infinity();

				lp->setValue(val * frac);
				double constraintNorm = std::numeric_limits<double>::infinity();
				unsigned int i = 0;
				for(i = 0; i < maxIterations; ++i) {
					constraintNorm = this->constraintRegister->getConstraintVectorNorm();

					if(std::isnan(constraintNorm)){
						break;
					}
					if(constraintNorm < minNorm){
						minNorm = constraintNorm;
						itWithMinNorm = i;
					}

					if((i - itWithMinNorm > 50) && (minNorm < 3 * startingMinNorm)) {
						break;
					}

					this->newtonMethod->iterate();
				}
				*output_stream<<i<<'\t'<<constraintNorm;
				this->printParameterValues(output_stream);
				*output_stream<< std::flush;
			}

			this->parameterRegister->updateValues(minNormValues);

			int secondPts = testPoints * (variationUpperBound - 1.0) / (variationUpperBound - variationLowerBound);
			for(const auto frac: getTestPoints(secondPts, 1.0, variationUpperBound)){
				uint itWithMinNorm = 0;
				double minNorm = std::numeric_limits<double>::infinity();

				lp->setValue(val * frac);
				double constraintNorm = std::numeric_limits<double>::infinity();
				unsigned int i = 0;
				for(i = 0; i < maxIterations; ++i) {
					constraintNorm = this->constraintRegister->getConstraintVectorNorm();

					if(std::isnan(constraintNorm)){
						break;
					}
					if(constraintNorm < minNorm){
						minNorm = constraintNorm;
						itWithMinNorm = i;
					}

					if((i - itWithMinNorm > 50) && (minNorm < 3 * startingMinNorm)) {
						break;
					}

					this->newtonMethod->iterate();
				}
				*output_stream<<i<<'\t'<<constraintNorm;
				this->printParameterValues(output_stream);
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
		void printFileHeader(std::ostream* output_stream) const {
			*output_stream<<"it\tconstr_norm";
			auto paramMap = this->parameterRegister->getParameterMap();
			for(const auto& i: paramMap){
				auto p = i.second.lock();
				*output_stream<<'\t'<<p->getName();
			}
			*output_stream<<std::endl;
		}
		void printParameterValues(std::ostream* output_stream) const {
			auto paramMap = this->parameterRegister->getParameterMap();
			for(const auto& i: paramMap){
				auto p = i.second.lock();
				*output_stream<<'\t'<<p->getValue();
			}
			*output_stream<<std::endl;
		}
};