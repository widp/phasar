/*
 * AllTop.hh
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_EDGE_FUNC_ALLTOP_HH_
#define ANALYSIS_IFDS_IDE_EDGE_FUNC_ALLTOP_HH_

#include <iostream>
#include <memory>
#include "../EdgeFunction.hh"

using namespace std;

template<typename V>
class AllTop : public EdgeFunction<V>, public enable_shared_from_this<AllTop<V>> {
private:
	const V topElement;

public:
	AllTop(V topElement) : topElement(topElement) { }

	virtual ~AllTop() = default;

	virtual V computeTarget(V source) override { return topElement; }

	virtual shared_ptr<EdgeFunction<V>> composeWith(shared_ptr<EdgeFunction<V>> secondFunction) override
	{
		return this->shared_from_this();
	}

	virtual shared_ptr<EdgeFunction<V>> joinWith(shared_ptr<EdgeFunction<V>> otherFunction) override
	{
		return otherFunction;
	}

	virtual bool equalTo(shared_ptr<EdgeFunction<V>> other) override
	{
		if (AllTop<V>* alltop = dynamic_cast<AllTop<V>*>(other.get()))
			return (alltop->topElement == topElement);
		return false;
	}

	friend ostream& operator<< (ostream& os, const AllTop& alltop)
	{
		return os << "alltop";
	}

	void dump() override { cout << "alltop\n"; }
};

#endif /* ANALYSIS_IFDS_IDE_EDGE_FUNC_ALLTOP_HH_ */