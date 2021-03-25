#pragma once

#include<iostream>
#include<iomanip>

class ParameterRegister {
	std::map<ParameterId, std::weak_ptr<Parameter>> parameterContainer;
	ParameterId nextId;
	public:
		ParameterRegister()
			: nextId(1)
			{}

		std::weak_ptr<Parameter> get(ParameterId id){
			return this->parameterContainer.find(id)->second;
		}

		std::map<ParameterId, std::weak_ptr<Parameter>>& getParameterMap(){
			return this->parameterContainer;
		}
		std::shared_ptr<Parameter> buildParameter(double initialValue, std::string units, std::string name){
			auto paramPtr = std::make_shared<Parameter>(this->getNextId(), initialValue, units, name);
			this->parameterContainer[paramPtr->getId()] = paramPtr;
			return paramPtr;
		}

		ParameterId getNextId(){
			return this->nextId++;
		}

		void printParameterTable(){
			std::cout<<std::setprecision(5)<<std::defaultfloat;
			for(const auto& parameterp: this->parameterContainer){
				const auto& lp = parameterp.second.lock();
				std::cout<<'\t'<<lp->getId()
						 <<'\t'<<lp->getName()
						 <<'\t'<<lp->getValue()
						 <<'\t'<<lp->getUnits()<<'\n';
			}
		}
};