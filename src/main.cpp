#include <iostream>
#include <filesystem>
#include <memory>
#include <map>
#include <math.h>
#include "Math/Constraint/Constraint.hpp"
#include "Math/Algebra/Jacobian/JacobianCalculator.hpp"
#include "Math/Algebra/GradientDescent/NewtonMethod.hpp"
#include "Component/Tank/Tank.hpp"


int main(int, char**) {
	auto JC = JacobianCalculator();
	auto GD = NewtonMethod(JC);

	auto pv = std::make_shared<Parameter>(1, 80.0);
	auto pP = std::make_shared<Parameter>(2, 0.9e5);
	auto pT = std::make_shared<Parameter>(3, 253);
	std::vector<std::weak_ptr<Parameter>> parameters = {pv,pP,pT};
	std::map<ParameterId, std::weak_ptr<Parameter>> parameterMap;
	for(const auto& p: parameters){
		parameterMap[p.lock()->getId()] = p;
	}

	auto boundary = std::make_shared<FluidBoundary>(1,pv,pP,pT,1);
	auto tank = Tank(1, {1,2}, boundary, 1e5, 273);
	std::vector<std::weak_ptr<Constraint>> constraints = tank.getConstraints();

	for(int i = 0; i < 20; ++i){
		std::cout<<"it: "<<i<<'\n';
		std::cout<<"\tParameters:\n";
		for(const auto& parameterp: parameters){
			const auto& lp = parameterp.lock();
			std::cout<<'\t'<<lp->getId()<<'\t'<<lp->getValue()<<'\n';
		}
		std::cout<<"\tConstraints:\n";
		for(const auto& constraintp: constraints){
			const auto& lc = constraintp.lock();
			std::cout<<'\t'<<lc->getId()<<'\t'<<lc->getValue()<<'\n';
		}
		GD.gradientDescentTick(parameterMap, constraints);
	}

	return 0;
}
