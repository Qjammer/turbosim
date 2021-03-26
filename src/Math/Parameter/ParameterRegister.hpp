#pragma once

#include<iostream>
#include<iomanip>
#include<fstream>

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

		void printParameterTable() const {
			std::cout<<std::setprecision(5)<<std::defaultfloat;
			for(const auto& parameterp: this->parameterContainer){
				const auto& lp = parameterp.second.lock();
				std::cout<<'\t'<<lp->getId()
						 <<'\t'<<lp->getName()
						 <<'\t'<<lp->getValue()
						 <<'\t'<<lp->getUnits()<<'\n';
			}
		}

		std::map<ParameterId, double> getValues() const {
			auto map = std::map<ParameterId, double>();

			for(const auto& parameterp: this->parameterContainer){
				const auto& lp = parameterp.second.lock();
				map[lp->getId()] = lp->getValue();
			}
			return map;
		}

		std::map<ParameterId, std::tuple<double, bool>> getParameterInfo() const {
			auto map = std::map<ParameterId, std::tuple<double, bool>>();

			for(const auto& parameterp: this->parameterContainer){
				const auto& lp = parameterp.second.lock();
				map[lp->getId()] = {lp->getValue(), lp->enable()};
			}
			return map;

		}

		void updateValues(const std::map<ParameterId, double>& newVals) const
		{
			for(const auto& valPack: newVals){
				const auto& parameterId = valPack.first;
				const auto& parameterVal = valPack.second;
				if(const auto& param = this->parameterContainer.find(parameterId); param != this->parameterContainer.end()){
					if(const auto& lp = param->second.lock()){
						lp->setValue(parameterVal);
					}
				}
			}
		}

		void loadParameterInfo(const std::map<ParameterId, std::tuple<double, bool>>& paramInfo) const
		{
			for(const auto& valPack: paramInfo){
				const auto& parameterId = valPack.first;
				const auto& parameterInfo = valPack.second;
				if(const auto& param = this->parameterContainer.find(parameterId); param != this->parameterContainer.end()){
					if(const auto& lp = param->second.lock()){
						lp->setValue(std::get<0>(parameterInfo));
						lp->enable(std::get<1>(parameterInfo));
					}
				}
			}
		}

		void loadFromFile(const std::string& filename) const {
			auto map = this->loadMapFromFile(filename);
			this->loadParameterInfo(map);
		}
		std::map<ParameterId, std::tuple<double, bool>> loadMapFromFile(const std::string& filename) const {
			auto map = std::map<ParameterId, std::tuple<double, bool>>();
			auto stream = std::ifstream(filename);
			if(!stream.is_open()){
				return map;
			}
			const char col_delim = '\t';
			const char row_delim = '\n';
			std::string row;
			while(std::getline(stream, row, row_delim)){
				std::stringstream ss(row);
				std::string id_str;
				if(!std::getline(ss, id_str, col_delim)){
					continue;
				}
				std::string val_str;
				if(!std::getline(ss, val_str, col_delim)){
					continue;
				}
				std::string enabled_str;
				if(!std::getline(ss, enabled_str, col_delim)){
					continue;
				}

				auto id = ParameterId(std::stol(id_str));
				auto val = double(std::stod(val_str));
				auto enabled = enabled_str == "1";

				map[id] = {val, enabled};
			}
			return map;
		}



		void saveMapToFile(const std::map<ParameterId, std::tuple<double, bool>>& map, const std::string& filename) const {
			auto stream = std::ofstream(filename);

			for(const auto& pair: map){
				stream
					<<std::setprecision(std::numeric_limits<long>::digits10)<<pair.first<<'\t'
					<<std::setprecision(std::numeric_limits<double>::digits10)<<std::get<0>(pair.second)<<'\t'
					<<std::setprecision(std::numeric_limits<bool>::digits10)<<std::get<1>(pair.second)<<'\n';
			}
		}
		void saveToFile(const std::string& filename) const {
			this->saveMapToFile(this->getParameterInfo(), filename);
		}
};