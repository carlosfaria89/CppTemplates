#pragma once

#include "attribute.h"

#include <unordered_map>

class generic_point_set
{
public:
    explicit generic_point_set(attribute_variant data, const size_t elements_per_point)
    {
        // Need to check for elements_per_point == 0 here to avoid SEH exception
        if(elements_per_point == 0)
        {
            throw std::invalid_argument("Invalid input");
        }
        const auto data_size = std::visit([](auto&& arg) { return arg.size(); }, data);
        point_count_ = data_size / elements_per_point;

        safeguard_attribute(data, elements_per_point);

        positions_ = attribute(std::move(data), elements_per_point);
    }

    template <typename T>
    [[nodiscard]] std::vector<T> get_positions() const
    {
        return std::get<std::vector<T>>(positions_.data);
    }

    [[nodiscard]] size_t get_point_count() const
    {
        return point_count_;
    }

    void add_attribute(const attribute_enum type, attribute_variant data, const size_t elements_per_point)
    {
        safeguard_attribute(data, elements_per_point);

        attributes_[type] = attribute(std::move(data), elements_per_point);
    }

    [[nodiscard]] std::vector<attribute_enum> get_all_attribute_types() const
    {
        auto keys = std::vector<attribute_enum>();
        keys.reserve(attributes_.size());
        for (const auto& [type, _] : attributes_)
        {
            keys.emplace_back(type);
        }
        return keys;
    }

    template <typename U>
    [[nodiscard]] std::vector<U> get_attribute_data(const attribute_enum type) const
    {
        return std::get<std::vector<U>>(attributes_.at(type).data);
    }

    [[nodiscard]] size_t get_attribute_dimensions(const attribute_enum type) const
    {
        return attributes_.at(type).dimensions;
    }

    void swap_attribute_data(const attribute_enum type, attribute_variant data)
    {
        const auto data_size = std::visit([](auto&& arg) { return arg.size(); }, data);
        const auto attribute_data_size = std::visit([](auto&& arg) { return arg.size(); }, attributes_.at(type).data);

        if(data_size != attribute_data_size)
        {
            throw std::invalid_argument("Data size mismatch");
        }

        std::swap(attributes_.at(type).data, data);
    }

    [[nodiscard]] bool has_attribute(const attribute_enum type) const
    {
        return attributes_.find(type) != attributes_.end();
    }

    void remove_attribute(const attribute_enum type)
    {
        if (has_attribute(type))
        {
            attributes_.erase(type);
        }
    }

    void clear_attributes()
    {
        attributes_.clear();
    }

    void sort_attributes(const std::vector<size_t>& ordered_indices)
    {
        if (ordered_indices.size() != point_count_)
        {
            throw std::invalid_argument("Ordered indices size does not match point count");
        }

        positions_.order(ordered_indices);
        for (auto& [attribute_type, attribute] : attributes_)
        {
            attribute.order(ordered_indices);
        }
    }

    void print() const
    {
        positions_.print();
        for (auto& [attribute_type, attribute] : attributes_)
        {
            attribute.print();
        }
    }

private:
    void safeguard_attribute(const attribute_variant& data, const size_t elements_per_point) const
    {
        const auto data_size = std::visit([](auto&& arg) { return arg.size(); }, data);

        if(data_size == 0 || elements_per_point == 0 || elements_per_point > data_size)
        {
            throw std::invalid_argument("Invalid input");
        }

        if(data_size % elements_per_point != 0)
        {
            throw std::invalid_argument("Remainder of attribute and elements per point is not zero");
        }

        if (data_size / elements_per_point != point_count_)
        {
            throw std::invalid_argument("Attribute point count does not match point set count");
        }
    }

private:
    size_t point_count_;

    attribute positions_;
    std::unordered_map<attribute_enum, attribute> attributes_;
};
