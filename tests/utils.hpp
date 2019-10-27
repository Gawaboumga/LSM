#ifndef LSM_UTILS_HPP
#define LSM_UTILS_HPP

#pragma once

#include <random>
#include <set>

namespace tests
{
	using random_int_type = typename std::uniform_int_distribution<>::result_type;
	using randomly_generated_type = std::pair<random_int_type, std::size_t>;

	std::vector<randomly_generated_type> generate_random_elements(std::size_t number_of_elements, float repetition_percent);
}

#endif // LSM_UTILS_HPP
