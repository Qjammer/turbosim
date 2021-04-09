#pragma once

#include "../../../Math/Algebra/NewtonMethod/NewtonMethod.hpp"
#include "../../../Math/Constraint/ConstraintRegister.hpp"
#include "../../../Math/Parameter/ParameterRegister.hpp"

class IterateUntilConverged {
		std::shared_ptr<NewtonMethod> newtonMethod;
		std::shared_ptr<ConstraintRegister> constraintRegister;
		std::shared_ptr<ParameterRegister> parameterRegister;
	public:
		IterateUntilConverged(
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
			uint maxIterations,
			std::string output_filename
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
			this->parameterRegister->updateValues(minNormValues);

			this->parameterRegister->saveToFile(output_filename);
		}


};