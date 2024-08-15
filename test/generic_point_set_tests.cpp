#include "gtest/gtest.h"

#include "generic_point_set.h"

TEST(GenericPointSetTests, ShouldAddAndGetPositions)
{
	const auto expected_positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	const auto points = generic_point_set(expected_positions, 3);
	const auto actual = points.get_positions<double>();

	EXPECT_EQ(expected_positions, actual);
}

TEST(GenericPointSetTests, PointCountShouldBeCorrect)
{
	const auto positions = std::vector<double>{1,2,3,4,5,6};

	const auto points = generic_point_set(positions, 3);

	EXPECT_EQ(points.get_point_count(), 2);
}

TEST(GenericPointSetTests, OtherAttributesShouldBeEmptyByDefault)
{
	const auto positions = std::vector<double>{1,2,3,4,5,6};

	const auto points = generic_point_set(positions, 3);

	EXPECT_EQ(points.get_all_attribute_types().empty(), true);
}

TEST(GenericPointSetTests, ShouldThrowIfInvalidInput)
{
	const auto positions = std::vector<double>{};

	EXPECT_THROW(generic_point_set(positions, 3), std::invalid_argument);
	EXPECT_THROW(generic_point_set(std::vector<double>{1.0, 2.0, 3.0}, 0), std::exception);
	EXPECT_THROW(generic_point_set(std::vector<double>{1.0, 2.0, 3.0}, 4), std::invalid_argument);
}

TEST(GenericPointSetTests, ShouldThrowIfRemainderOfPositionsAndElementsPerPointIsNotZero)
{
	const auto expected = std::vector<double>{1,2,3};

	EXPECT_THROW(generic_point_set(expected, 2), std::invalid_argument);
}

TEST(GenericPointSetTests, ShouldBeAbleToAddKnownAttribute)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);
	std::vector<int> expected_indices{1, 2, 3};
	points.add_attribute(attribute_enum::index, expected_indices, 1);

	const auto attributes = points.get_all_attribute_types();
	const auto actual = points.get_attribute_data<int>(attribute_enum::index);

	EXPECT_EQ(attributes.front(), attribute_enum::index);
	EXPECT_EQ(actual, expected_indices);
}

TEST(GenericPointSetTests, ShouldBeAbleToDeleteAttribute)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);
	std::vector<int> expected_indices{1, 2, 3};
	points.add_attribute(attribute_enum::index, expected_indices, 1);

	const auto attributes = points.get_all_attribute_types();
	const auto actual = points.get_attribute_data<int>(attribute_enum::index);

	EXPECT_EQ(attributes.front(), attribute_enum::index);

	points.remove_attribute(attribute_enum::index);

	EXPECT_EQ(points.get_all_attribute_types().size(), 0);
}

TEST(GenericPointSetTests, ShouldThrowIfTryingToGetANonExistantAttribute)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);
	std::vector<int> expected_indices{1, 2, 3};
	points.add_attribute(attribute_enum::index, expected_indices, 1);

	const auto attributes = points.get_all_attribute_types();
	EXPECT_THROW(auto _ = points.get_attribute_data<float>(attribute_enum::normal), std::out_of_range);
	EXPECT_THROW(auto _ = points.get_attribute_dimensions(attribute_enum::normal), std::out_of_range);
}

TEST(GenericPointSetTests, SwapAttributeShouldWork)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);
	points.add_attribute(attribute_enum::index, std::vector<int>{9,9,9}, 1);

	std::vector<int> expected_indices{1, 2, 3};
	points.swap_attribute_data(attribute_enum::index, expected_indices);

	const auto actual = points.get_attribute_data<int>(attribute_enum::index);

	EXPECT_EQ(actual, expected_indices);
}

TEST(GenericPointSetTests, ShouldNotBeAbleToAddAttributeDataThatDoesNotMatchThePointCount)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);

	EXPECT_THROW(
		points.add_attribute(attribute_enum::index, std::vector<int>{9,9}, 1),
		std::invalid_argument);
	EXPECT_THROW(
		points.add_attribute(attribute_enum::index, std::vector<int>{9,9,9}, 2),
		std::invalid_argument);
	EXPECT_THROW(
		points.add_attribute(attribute_enum::index, std::vector<int>{9,9,9}, 4),
		std::invalid_argument);
}

TEST(GenericPointSetTests, ShouldNotBeAbleToSwapInvalidData)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);
	points.add_attribute(attribute_enum::index, std::vector<int>{9,9,9}, 1);

	EXPECT_THROW(points.swap_attribute_data(attribute_enum::index, std::vector<int>{9,9}), std::invalid_argument);
}

TEST(GenericPointSetTests, SortShouldSortAllAttributes)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);
	points.add_attribute(attribute_enum::index, std::vector<int>{1,2,3}, 1);

	points.sort_attributes(std::vector<size_t>{2, 1, 0});

	const auto expected_positions = std::vector<double>{7, 8, 9, 4, 5, 6, 1, 2, 3};
	EXPECT_EQ(points.get_positions<double>(), expected_positions);

	const auto expected_indices = std::vector<int>{3, 2, 1};
	EXPECT_EQ(points.get_attribute_data<int>(attribute_enum::index), expected_indices);
}

TEST(GenericPointSetTests, ShouldThrowIfIndicesCountDiffersFromPointCount)
{
	const auto positions = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto points = generic_point_set(positions, 3);

	EXPECT_THROW(points.sort_attributes(std::vector<size_t>{2, 0}), std::invalid_argument);
}
