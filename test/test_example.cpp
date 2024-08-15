#include "gtest/gtest.h"
#include "example.h"

TEST(PersonTest, ConstructorValidArguments)
{
	// Arrange
	std::string testName = "John";
	int testAge = 25;

	// Act
	Person person(testName, testAge);

	// Assert
	EXPECT_EQ(person.getName(), testName);
	EXPECT_EQ(person.getAge(), testAge);
}

TEST(PersonTest, ConstructorInvalidAge)
{
	// Arrange
	std::string testName = "John";
	int invalidAge = -5;

	// Act and Assert
	EXPECT_THROW(Person(testName, invalidAge), std::invalid_argument);
}