#pragma once

#include <string>

class Person {
public:
	Person(std::string name, int age);

	std::string getName() const;
	int getAge() const;

private:
	std::string _name;
	int _age;
};