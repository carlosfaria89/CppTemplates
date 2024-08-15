#include <iostream>
#include <vector>

#include "GenericPointSet.h"

int main(int argc, char* argv[])
{
	auto points_raw = GenericPointSet(std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9}, 3);

	points_raw.AddAttribute(attribute_enum::index, std::vector<int>{1, 2, 3}, 1);
	// points_raw.AddAttribute(attribute_enum::normal, new float[]{0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f}, 3);

	const auto attributes = points_raw.GetAllAttributeTypes();

	points_raw.SortAttributes(std::vector<size_t>{2,1,0});

	const auto raw_indices = points_raw.GetAttributeData(attribute_enum::index);

	std::cout << "Indices: " <<
		raw_indices[0] << " " <<
		raw_indices[1] << " " <<
		raw_indices[2] << " " <<
			std::endl;
	// auto raw_qualities = points_raw.GetAttribute<float>(attribute_enum::normal);

	// points_raw.Print();

	// SWAP only swaps first item

	return 0;
}
