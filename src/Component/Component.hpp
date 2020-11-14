#pragma once
#include <vector>
#include <memory>
#include "../Boundary/Boundary.hpp"

class Component{
		int id;
	public:
		Component(int id);
		virtual int getId() const {return this->id;};
		virtual const Boundary& getBoundary(int i) const = 0;
		virtual Boundary& getBoundary(int i) = 0;
		virtual std::vector<std::weak_ptr<Boundary>> getBoundaries() = 0;
		virtual int getInvariantCount() const = 0;
		virtual float getInvariant(int i) const = 0;
		virtual float getInvariantDerivative(int invariant, int parameter) const = 0;
};