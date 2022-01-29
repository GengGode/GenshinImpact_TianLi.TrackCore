#pragma once
#include <memory>
#include <any>

//class DataFacility
//{
//	std::any _value;
//public:
//	virtual void setOutputValue(std::any value)=0;
//	virtual std::any getOutputValue() = 0;
//};
//
//using Data = std::unique_ptr<DataFacility>;

template <typename T>
class DataFacility
{
	T _value;
public:
	virtual void setValue(T value) = 0;
	virtual T getValue() = 0;
};

template <typename T>
using Data = std::unique_ptr<DataFacility<T>>;