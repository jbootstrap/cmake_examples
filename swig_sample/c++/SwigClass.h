#pragma once

#include <string>
#include <vector>
#include "ResultClass.h"

template<class T> class myvector {
	T *v;
	int sz;
public:
	myvector(int _sz) {
		v = new T[_sz];
		sz = _sz;
	}
	T &get(int index) {
		return v[index];
	}
	void set(int index, T &val) {
		v[index] = val;
	}
#ifdef SWIG
	%extend{
		T getitem(int index) {
			return $self->get(index);
		}
		void setitem(int index, T val) {
			$self->set(index, val);
		}
	}
#endif
};

class SwigClass
{
public:
	SwigClass();
	virtual ~SwigClass();
	int add2(int a, int b);
	std::wstring getString();
	static int getClassCount();
	ResultClass *getResult()
	{
		return new ResultClass();
	}
	std::vector<int> getIntVector()
	{
		std::vector<int> result;
		result.push_back(11);
		result.push_back(22);
		return result;
	}
};

void setString(const std::wstring &s);

int getClassCount();
