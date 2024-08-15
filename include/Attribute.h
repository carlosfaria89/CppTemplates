#pragma once

#include <iostream>
#include <variant>
#include <vector>

// Supported attribute enum
enum class attribute_enum
{
    index,
    normal,
    quality,
    visibility,
};


// Supported attribute types
using attribute_variant = std::variant<
    std::vector<double>,
    std::vector<float>,
    std::vector<int>,
    std::vector<uint32_t>,
    std::vector<uint16_t>,
    std::vector<uint8_t>,
    std::vector<bool>>;

struct attribute
{
    attribute_variant data;
    size_t dimensions{};

    attribute() = default;

    attribute(attribute_variant var, const size_t elements_per_point) :
        data(std::move(var)),
        dimensions(elements_per_point)
    {
    }

    void order(const std::vector<size_t>& indices)
    {
        const auto dim = dimensions;
        std::visit([indices, dim](auto&& arg)
        {
            using V = typename std::decay_t<decltype(arg)>::value_type;
            std::vector<V> ordered(indices.size() * dim);
            for (size_t i = 0; i < indices.size(); ++i)
            {
                for (size_t j = 0; j < dim; ++j)
                {
                    ordered[i * dim + j] = arg[indices[i] * dim + j];
                }
            }

            std::swap(arg, ordered);
        }, data);
    }

    void print() const
    {
        const auto dim = dimensions;
        std::visit([dim](auto&& arg)
        {
            for (size_t i = 0; i < dim; ++i)
            {
                for (size_t j = 0; j < arg.size() / dim; ++j)
                {
                    std::cout << arg[i * dim + j] << " ";
                }
                std::cout << "\n";
            }
        }, data);
    }
};
