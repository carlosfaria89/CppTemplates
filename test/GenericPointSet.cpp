#include "gtest/gtest.h"

#include "GenericPointSet.h"

// TEST(GenericPointSetTests, CanAddAndGetPositions)
// {
// 	auto points_raw = GenericPointSet(new double[]{1, 2, 3, 4, 5, 6, 7, 8, 9}, 3);
//
// 	points_raw.AddAttribute(attribute_enum::int_index, new int[]{1, 2, 3}, 1);
// 	// points_raw.AddAttribute(attribute_enum::normal, new float[]{0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f}, 3);
//
// 	const auto attributes = points_raw.GetAllAttributeTypes();
//
// 	points_raw.SortAttributes(std::vector<size_t>{2,1,0});
//
// 	// auto positions = points_raw.GetPositions();
// 	auto raw_indices = points_raw.GetAttribute<int>(attribute_enum::int_index);
// 	// auto raw_qualities = points_raw.GetAttribute<float>(attribute_enum::normal);
//
// 	// points_raw.Print();
//
// 	// SWAP only swaps first item
//
// 	return 0;
//
// 	// Assert
// 	EXPECT_EQ(person.getName(), testName);
// 	EXPECT_EQ(person.getAge(), testAge);
// }