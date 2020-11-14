#include <iostream>
#include <filesystem>
#include <memory>
#include <map>
#include <math.h>
#include <Eigen/Dense>
#include "Math/Constraint/Constraint.hpp"
#include "Math/Algebra/Jacobian/JacobianCalculator.hpp"

class SimpleConstraint: public Constraint{
	std::shared_ptr<Parameter> p1;
	std::shared_ptr<Parameter> p2;
	public:
		SimpleConstraint(int id, std::shared_ptr<Parameter> p1, std::shared_ptr<Parameter> p2)
			:Constraint(id),p1(p1), p2(p2){
		}
		float getValue() const {
			return p1->getValue() - p2->getValue();
		}
		std::vector<std::weak_ptr<Parameter>> getDependentParameters() const {
			return {p1, p2};
		}

		float getValueDerivative(const Parameter& p) const {
			if(p.getId() == this->p1->getId()) {
				return 1.0;
			}
			if(p.getId() == this->p2->getId()) {
				return -1.0;
			}
			return 0.0;
		}
};

int main(int, char**) {
	auto JC = JacobianCalculator();
	auto p1 = std::make_shared<Parameter>(1, 0.0);
	auto p2 = std::make_shared<Parameter>(2, 1.0);

	auto c1 = std::make_shared<SimpleConstraint>(1,p1,p2);
	const std::vector<std::weak_ptr<const Constraint>> constraints = {c1};
	std::cout<<"Id"<<"\t"<<"Val"<<"\t"<<"dc/dp"<<'\n';
	for(auto param: c1->getDependentParameters()){
		auto lp = param.lock();
		std::cout<<lp->getId()<<"\t"<<lp->getValue()<<"\t"<<c1->getValueDerivative(*lp)<<'\n';
	}
	std::cout<<std::flush;

	for(auto valuePair:JC.getConstraintValues(constraints)){
		std::cout<< valuePair.first << " " <<valuePair.second<< '\n';
	}

	std::cout<< "c_id\tp_id\tdcdp\n";
	for(auto vp1:JC.getSparseJacobian(constraints)){
		for(auto vp2: vp1.second){
			std::cout<< vp1.first << '\t' <<vp2.first<<'\t'<<vp2.second<< '\n';
		}
	}
	return 0;
}
