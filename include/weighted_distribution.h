#ifndef DEF_WEIGHTED_DISTRIBUTION_H
#define DEF_WEIGHTED_CHANCE_STRUCTURE_H
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>

template <class T>
struct weighted_distribution
{
    using weight_type = double;
    std::vector<std::pair<T, weight_type>> weighted_struct;
public:
    weighted_distribution() : weighted_struct{}{}
    weighted_distribution(const std::vector<std::pair<T, weight_type>>& weighted_struct) :  weighted_struct{ weighted_struct.begin(),weighted_struct.end() } {}

    T operator()(std::default_random_engine& engine)
    {
        weight_type total = std::accumulate(weighted_struct.begin(), weighted_struct.end(), 0.0, [](weight_type current_total, std::pair<T, weight_type> value) {
            return current_total += value.second;
        });
        
        std::uniform_real_distribution<> distrubution(0, total);
        weight_type rnd = distrubution(engine);
        T return_value;
        for (auto& v: weighted_struct)
        {
            if (rnd < v.second)
            {
                return_value = v.first;
                break;
            }
            rnd -= v.second;
        }

        return return_value;
    }
};
#endif // !DEF_WEIGHTED_CHANCE_STRUCTURE_H
