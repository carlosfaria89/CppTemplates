#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <variant>

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
    std::vector<int>,
    std::vector<float>,
    std::vector<uint32_t>,
    std::vector<uint8_t>,
    std::vector<bool>>;

struct Attribute
{
    attribute_variant data;
    size_t dimensions{};

    Attribute() = default;

    Attribute(attribute_variant var, const size_t elements_per_point) :
        data(std::move(var)),
        dimensions(elements_per_point)
    {
    }

    void Order(const std::vector<size_t>& indices)
    {
        const auto dim = dimensions;
        std::visit([indices, dim](auto&& arg)
        {
            using V = std::decay_t<decltype(arg)>;
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
};

template <typename T>
class GenericPointSet
{
public:
    explicit GenericPointSet(std::vector<T> pos, const size_t elements_per_point) :
        positions_(std::move(pos), elements_per_point),
        point_count_(pos/elements_per_point)
    {
    }

    [[nodiscard]] std::vector<T> GetPositions() const
    {
        return positions_;
    }

    [[nodiscard]] size_t GetPointCount() const
    {
        return point_count_;
    }

    void AddAttribute(const attribute_enum type, attribute_variant data, const size_t elements_per_point)
    {
        attributes_[type] = Attribute(std::move(data), elements_per_point);
    }

    [[nodiscard]] std::vector<attribute_enum> GetAllAttributeTypes() const
    {
        auto keys = std::vector<attribute_enum>();
        keys.reserve(attributes_.size());
        for (const auto& [type, _] : attributes_)
        {
            keys.emplace_back(type);
        }
        return keys;
    }

    // template <typename U>
    // [[nodiscard]] std::vector<U> GetAttributeData(const attribute_enum type) const
    // {
    //     return std::get<std::vector<U>>(attributes_.at(type).data);
    // }

    [[nodiscard]] auto GetAttributeData(const attribute_enum type) const
    {
        return std::visit([](auto&& arg) -> std::decay_t<decltype(arg)> {
            return arg;
        }, attributes_.at(type).data);
    }

    [[nodiscard]] size_t GetAttributeDimensions(const attribute_enum type) const
    {
        return attributes_.at(type).dimensions;
    }

    void SwapAttributeData(const attribute_enum type, attribute_variant data)
    {
        std::swap(attributes_.at(type).data, data);
    }

    void SortAttributes(const std::vector<size_t>& ordered_indices)
    {
        if (ordered_indices.size() != point_count_)
        {
            return;
        }

        positions_.Order(ordered_indices);
        for (auto& [attribute_type, attribute] : attributes_)
        {
            attribute.Order(ordered_indices);
        }
    }

private:
    Attribute positions_;
    std::unordered_map<attribute_enum, Attribute> attributes_;

    size_t point_count_;
};
