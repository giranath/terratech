#ifndef DEF_MAPGEN_BIOME_TABLE_HPP
#define DEF_MAPGEN_BIOME_TABLE_HPP

#include <vector>

namespace mapgen {

class biome_axis {
public:
    using index = std::size_t;
    using biome_type = int;

    static const biome_type NO_SPECIAL_BIOME = biome_type{};
private:
    std::vector<biome_type> biomes;
public:
    biome_axis() = default;
    biome_axis(index count, biome_type default_biome = NO_SPECIAL_BIOME);

    void set_biome_at(index i, biome_type biome) noexcept;
    biome_type biome_at(index i) const noexcept;
    index count() const noexcept;

    template<typename PERCENT>
    biome_type biome_with(PERCENT percent) const noexcept {
        static_assert(std::is_floating_point<PERCENT>::value, "floating type is required");

        return biome_at((count() - 1) * percent);
    }
};

class biome_table {
public:
    using column_type = std::size_t;
    using row_type = std::size_t;
    using biome_type = int;
private:
    std::vector<std::vector<biome_type>> biomes;

public:
    biome_table() = default;
    biome_table(column_type col_count, row_type row_count, biome_type default_biome = {});

    void set_biome_at(column_type col, row_type row, biome_type biome) noexcept;
    biome_type biome_at(column_type col, row_type row) const noexcept;

    row_type row_count() const noexcept;
    column_type col_count() const noexcept;

    template<typename PERCENT>
    biome_type biome_with(PERCENT col, PERCENT row) const noexcept {
        static_assert(std::is_floating_point<PERCENT>::value, "floating type is required");
        return biome_at((col_count() - 1) * col, (row_count() - 1) * row);
    }
};

}

#endif
