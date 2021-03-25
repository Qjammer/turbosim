#pragma once

#include<iostream>
#include<iomanip>

class ConstraintRegister {
	std::map<ConstraintId, std::weak_ptr<Constraint>> constraintContainer;
	ConstraintId nextId;
	public:
		ConstraintRegister()
			: nextId(1)
			{}

		ConstraintId getNextId(){
			return this->nextId++;
		}

		template<int N>
		std::array<ConstraintId, N> getNextIds(){
			std::array<ConstraintId, N> a{};
			for(int i = 0; i<N; ++i){
				a[i] = this->getNextId();
			}
			return a;

		}

		template <typename T>
		void registerComponent(const T& c) {
			auto constraints = c.getConstraints();
			for(const auto& c: constraints){
				this->registerConstraint(c.lock());
			}
		}

		std::map<ConstraintId, std::weak_ptr<Constraint>>& getAllConstraints() {
			return this->constraintContainer;
		}

		void registerConstraint(std::shared_ptr<Constraint> constraint){
			this->constraintContainer[constraint->getId()] = constraint;
		}

		void printConstraintTable() const {
			std::cout<<std::setprecision(5)<<std::defaultfloat;
			for(const auto& constraintp: this->constraintContainer){
				const auto& lc = constraintp.second.lock();
				std::cout<<'\t'<<lc->getId()<<'\t'<<lc->getValue()<<'\n';
			}
			std::cout<<'\t'<<"Norm:"<<'\t'<<this->getConstraintVectorNorm()<<'\n';
		}

		double getConstraintVectorNorm() const {
			double norm = 0.0;
			for(const auto& constraintp: this->constraintContainer){
				norm += powf(constraintp.second.lock()->getValue(), 2);
			}
			return sqrt(norm);
		}
};