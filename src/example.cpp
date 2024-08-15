#include "example.h"

#include <stdexcept>

Person::Person(std::string name, int age)
{
	if (age < 0)
	{
		throw std::invalid_argument("Age needs to be a non-negative integer");
	}

	_name = name;
	_age = age;
}

std::string Person::getName() const
{
	return _name;
}

int Person::getAge() const
{
	return _age;
}
