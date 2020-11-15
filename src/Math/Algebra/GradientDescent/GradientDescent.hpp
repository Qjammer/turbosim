#pragma once

class GradientDescent {
		float l = 0.1;
		JacobianCalculator jacobianCalculator;
	public:
		GradientDescent(JacobianCalculator jacobianCalculator)
			:jacobianCalculator(jacobianCalculator)
		{
		}

		void gradientDescentTick(std::map<ParameterId, std::weak_ptr<Parameter>>& parameters, const std::vector<std::weak_ptr<Constraint>>& constraints) {
			const auto& constraintValues = this->jacobianCalculator.getConstraintValues(constraints);
			const auto& constraintJacobian = this->jacobianCalculator.getJacobian(constraints);
			const auto& CdCdP = this->getCdCdP(constraintValues, constraintJacobian);
			const auto& parameterValues = this->getParameterValues(parameters);
			const auto& updatedParameterValues = this->getUpdatedParameterValues(parameterValues, CdCdP);
			this->updateParams(parameters, updatedParameterValues);
		}

		std::map<ParameterId, float> getCdCdP(const std::map<ConstraintId, float>& constraintValues, const std::map<ConstraintId, std::map<ParameterId, float>>& jacobian) const
		{
			std::map<ParameterId, float> CdCdP;
			for(const auto& vp1: jacobian){
				const auto& constraintId = vp1.first;
				const auto& constraintGradient = vp1.second;
				const auto& constraintValuePair = constraintValues.find(constraintId);
				if(constraintValuePair != constraintValues.end()){
					const auto& constraintValue = constraintValuePair->second;
					for(const auto& vp2: constraintGradient){
						const auto& parameterId = vp2.first;
						const auto& dCdP = vp2.second;
						CdCdP[parameterId] += dCdP * constraintValue;
					}
				}
			}
			return CdCdP;
		}


		void updateParams(std::map<ParameterId, std::weak_ptr<Parameter>>& parameters, const std::map<ParameterId, float>& newVals) const
		{
			for(const auto& valPack: newVals){
				const auto& parameterId = valPack.first;
				const auto& parameterVal = valPack.second;
				if(const auto& param = parameters.find(parameterId); param != parameters.end()){
					if(const auto& lp = param->second.lock()){
						lp->setValue(parameterVal);
					}
				}
			}

		}

		std::map<ParameterId, float> getParameterValues(const std::map<ParameterId, std::weak_ptr<Parameter>>& parameters) const
		{
			std::map<ParameterId, float> values;
			for(const auto& parameterPair: parameters){
				const auto& parameterId = parameterPair.first;
				const auto& parameterWP = parameterPair.second;
				if(const auto& lp = parameterWP.lock()){
					values[parameterId] = lp->getValue();
				}
			}

			return values;
		}

		std::map<ParameterId, float> getUpdatedParameterValues(const std::map<ParameterId, float>& parameterValues, const std::map<ParameterId, float>& CdCdP) const
		{
			std::map<ParameterId, float> newVals;
			for(const auto& parameterPack: parameterValues){
				const auto& parameterId = parameterPack.first;
				if(const auto& paramCdCdP = CdCdP.find(parameterId); paramCdCdP != CdCdP.end()){
					newVals[parameterId] = parameterPack.second - this->l * paramCdCdP->second;
				}
			}
			return newVals;
		}
};