#include "../include/biome_table.hpp"

namespace mapgen {

biome_axis::biome_axis(index count, biome_type default_biome)
: biomes(count, default_biome) {

}

void biome_axis::set_biome_at(index i, biome_type biome) noexcept {
    biomes[i] = biome;
}

biome_axis::biome_type biome_axis::biome_at(index i) const noexcept {
    return biomes[i];
}

biome_axis::index biome_axis::count() const noexcept {
    return biomes.size();
}

biome_table::biome_table(column_type col_count, row_type row_count, biome_type default_biome)
: biomes{} {
	biomes.resize(row_count);
	for(std::size_t i = 0; i < row_count; ++i) {
		biomes[i].resize(col_count, default_biome);
	}
}

void biome_table::set_biome_at(column_type col, row_type row, biome_type biome) noexcept {
	biomes[row][col] = biome;
}

biome_table::biome_type biome_table::biome_at(column_type col, row_type row) const noexcept {
	return biomes[row][col];
}

biome_table::row_type biome_table::row_count() const noexcept {
	return biomes.size();
}

biome_table::column_type biome_table::col_count() const noexcept {
	if(row_count() > 0) {
		return biomes[0].size();
	}
	
	return 0;
}

}
