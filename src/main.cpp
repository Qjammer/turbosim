#include <iostream>
#include <filesystem>
#include <memory>
#include <map>
#include <math.h>
#include <Eigen/Dense>
#include "Math/Constraint/Constraint.hpp"
#include "Math/Algebra/Jacobian/JacobianCalculator.hpp"
#include "Math/Algebra/GradientDescent/GradientDescent.hpp"

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
	auto GD = GradientDescent(JC);

	auto p1 = std::make_shared<Parameter>(1, 0.0);
	auto p2 = std::make_shared<Parameter>(2, 1.0);
	std::vector<std::weak_ptr<Parameter>> parameters = {p1,p2};
	std::map<ParameterId, std::weak_ptr<Parameter>> parameterMap;
	for(const auto& p: parameters){
		parameterMap[p.lock()->getId()] = p;
	}

	auto c1 = std::make_shared<SimpleConstraint>(1,p1,p2);
	std::vector<std::weak_ptr<Constraint>> constraints = {c1};

	for(int i = 0; i < 100; ++i){
		GD.gradientDescentTick(parameterMap, constraints);
	}

	return 0;
}
