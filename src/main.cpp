#include <iostream>
#include <filesystem>
#include <memory>
#include <map>
#include <math.h>
#include "Math/Constraint/Constraint.hpp"
#include "Math/Algebra/Jacobian/JacobianCalculator.hpp"
#include "Math/Algebra/GradientDescent/NewtonMethod.hpp"
#include "Component/Tank/Tank.hpp"
#include "Component/Turbine/Turbine.hpp"


int main(int, char**) {

	auto pv1 = std::make_shared<Parameter>(1, 20.0);
	auto pP1 = std::make_shared<Parameter>(2, 1e5);
	auto pT1 = std::make_shared<Parameter>(3, 253);

	auto pv2 = std::make_shared<Parameter>(4, 20.0);
	auto pP2 = std::make_shared<Parameter>(5, 0.9e5);
	auto pT2 = std::make_shared<Parameter>(6, 253);

	auto pw = std::make_shared<Parameter>(7, 0.0);
	auto pM = std::make_shared<Parameter>(8, 0.0);
	std::vector<std::weak_ptr<Parameter>> parameters = {pv1,pP1,pT1,pv2,pP2,pT2};//,pw,pM};
	std::map<ParameterId, std::weak_ptr<Parameter>> parameterMap;
	for(const auto& p: parameters){
		parameterMap[p.lock()->getId()] = p;
	}

	auto fluidBoundary1 = std::make_shared<FluidBoundary>(1,pv1,pP1,pT1,1.0);
	auto fluidBoundary2 = std::make_shared<FluidBoundary>(2,pv2,pP2,pT2,1.0);
	auto axialBoundary1 = std::make_shared<AxialBoundary>(3,pw,pM);
	auto tank1 = Tank(1, {1,2}, 1e5, 273);
	tank1.registerFluidBoundary(fluidBoundary1);
	auto tank2 = Tank(2, {3,4}, 0.9e5, 273);
	tank2.registerFluidBoundary(fluidBoundary2);

	auto turbine = Turbine(3, {5,6,7});
	turbine.registerFluidBoundary(fluidBoundary1, 0);
	turbine.registerFluidBoundary(fluidBoundary2, 1);
	turbine.registerAxialBoundary(axialBoundary1, 0);

	std::vector<std::weak_ptr<Constraint>> constraints;
	auto constraints1 = tank1.getConstraints();
	constraints.insert(constraints.end(), constraints1.begin(), constraints1.end());
	auto constraints2 = tank2.getConstraints();
	constraints.insert(constraints.end(), constraints2.begin(), constraints2.end());
	auto constraints3 = turbine.getConstraints();
	constraints.insert(constraints.end(), constraints3.begin(), constraints3.end());


	auto JC = JacobianCalculator();
	auto GD = NewtonMethod(JC);

	for(int i = 0; i < 100; ++i){
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
