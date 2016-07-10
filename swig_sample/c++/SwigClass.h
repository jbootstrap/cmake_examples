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
	void setBytes(const std::string &bytes, std::string bytes2);
	void setBytes2(std::string bytes);
	std::string getBytes();
	const std::string &getBytes2();
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
	std::wstring getUserEnvVar(const std::wstring &name);
	void setUserEnvVar(const std::wstring &name, const std::wstring &value);
};

void setString(const std::wstring &s);

int getClassCount();
