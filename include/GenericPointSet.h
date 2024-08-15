#pragma once

#include <cstdint>
#include <unordered_map>
#include <variant>

// Supported attribute enum
enum class attribute_enum
{
    int_index,
    normal,
    compressed_normal,
    byte_quality,
    bool_visibility,
};

// Supported attribute types
using attribute_variant = std::variant<
    int*,
    float*,
    uint32_t*,
    uint8_t*,
    bool*>;

struct Attribute
{
    attribute_variant data;
    size_t dimensions{};

    Attribute() = default;

    Attribute(const attribute_variant& var, const size_t elements_per_point) :
        data(var),
        dimensions(elements_per_point)
    {
    }

    void OrderData(const std::vector<size_t>& indices)
    {
        const auto dim = dimensions;
        std::visit([indices, dim](auto&& arg)
        {
            using V = std::decay_t<decltype(arg)>;
            auto ordered = new V[indices.size() * dim];
            for (size_t i = 0; i < indices.size(); ++i)
            {
                for (size_t j = 0; j < dim; ++j)
                {
                    ordered[i * dim + j] = &arg[indices[i] * dim + j];
                }
            }

            std::swap(arg, *ordered);
        }, data);
    }
};

template <typename T>
class GenericPointSet
{
public:
    explicit GenericPointSet(T* pos, size_t point_count) :
        positions_(pos),
        point_count_(point_count)
    {
    }

    [[nodiscard]] T* GetPositions() const
    {
        return positions_;
    }

    void AddAttribute(const attribute_enum type, const attribute_variant& data, const size_t dimensions)
    {
        attributes_[type] = Attribute(data, dimensions);
    }

    void AddAttribute(const attribute_enum type, const attribute_variant&& data, const size_t dimensions)
    {
        attributes_[type] = Attribute(data, dimensions);
    }


    template <typename U>
    [[nodiscard]] U* GetAttribute(const attribute_enum type) const
    {
        return std::get<U*>(attributes_.at(type).data);
    }

    void SwapAttribute(const attribute_enum type, attribute_variant& data)
    {
        std::swap(attributes_.at(type).data, data);
    }

    [[nodiscard]] size_t GetAttributeDimensionality(const attribute_enum type) const
    {
        return attributes_.at(type).dimensions;
    }

    [[nodiscard]] size_t GetPointCount() const
    {
        return point_count_;
    }

    [[nodiscard]] std::vector<attribute_enum> GetAllAttributeTypes() const
    {
        auto keys = std::vector<attribute_enum>();
        keys.reserve(attributes_.size());
        for (auto [type, _] : attributes_)
        {
            keys.emplace_back(type);
        }
        return keys;
    }

    void SortAttributes(const std::vector<size_t>& ordered_indices)
    {
        if (ordered_indices.size() != point_count_)
        {
            return;
        }

        auto ordered_positions = OrderData<T>(ordered_indices, positions_, 3);
        std::swap(positions_, ordered_positions);

        for (auto& [attribute_type, attribute] : attributes_)
        {
            attribute.OrderData(ordered_indices);
        }
    }

    void Print() const
    {
        for (size_t i = 0; i < point_count_; ++i)
        {
            std::cout << "Position: " << i << " ";
            for (size_t j = 0; j < 3; ++j)
            {
                std::cout << positions_[i * 3 + j] << " ";
            }
            std::cout << std::endl;
        }

        for (auto [type, attribute]: attributes_)
        {
            std::cout << "Attribute: " << static_cast<int>(type) << std::endl;
            for (size_t i = 0; i < point_count_; ++i)
            {
                const auto dim = attribute.dimensions;
                for (size_t j = 0; j < dim; ++j)
                {
                    std::visit([i, j, dim](auto&& arg)
                    {
                        std::cout << arg[i * dim + j] << " ";
                    }, attribute.data);
                }
                std::cout << std::endl;
            }
        }
    }

private:
    template <typename V>
    [[nodiscard]] static V* OrderData(const std::vector<size_t>& indices, V* data, const size_t dimensions)
    {
        auto ordered = new V[indices.size() * dimensions];
        for (size_t i = 0; i < indices.size(); ++i)
        {
            for (size_t j = 0; j < dimensions; ++j)
            {
                ordered[i * dimensions + j] = data[indices[i] * dimensions + j];
            }
        }
        return ordered;
    }

    T* positions_;
    std::unordered_map<attribute_enum, Attribute> attributes_;

    size_t point_count_;
};
