#ifndef DEF_MAPGEN_BIOME_TABLE_HPP
#define DEF_MAPGEN_BIOME_TABLE_HPP

#include <vector>

namespace mapgen {

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
