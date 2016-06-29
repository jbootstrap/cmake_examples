//#include "stdafx.h"
#include "SwigClass.h"
#include <stdio.h>

static int s_class_count = 0;

SwigClass::SwigClass()
{
	s_class_count++;
	printf("SwigClass::SwigClass(): %d\n", s_class_count);
}


SwigClass::~SwigClass()
{
	s_class_count--;
	printf("SwigClass::~SwigClass(): %d\n", s_class_count);
}

int SwigClass::add2(int a, int b)
{
	return a + b;
}

std::wstring SwigClass::getString()
{
	return L"returnstr漢字";
}

int SwigClass::getClassCount()
{
	return s_class_count;
}

void setString(const std::wstring &s)
{
	setlocale(LC_ALL, "Japanese");
	printf("setString(): s=%ls\n", s.c_str());
}

int getClassCount()
{
	return s_class_count;
}
