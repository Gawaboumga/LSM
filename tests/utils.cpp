#include "utils.hpp"

namespace tests
{
	std::vector<randomly_generated_type> generate_random_elements(std::size_t number_of_elements, float repetition_percent)
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		static std::mt19937 gen(0);
		std::uniform_int_distribution<> dis;

		std::size_t number_of_elements_with_repetitions = number_of_elements * repetition_percent;
		std::size_t number_of_elements_without_repetitions = number_of_elements * (1.f - repetition_percent);

		std::set<random_int_type> uniques;
		std::vector<randomly_generated_type> results;
		std::size_t counter = 0;
		while (counter < number_of_elements_without_repetitions)
		{
			auto number = dis(gen);
			auto pair = uniques.insert(number);
			if (pair.second)
			{
				results.push_back({ number, counter });
				++counter;
			}
		}

		std::uniform_int_distribution<> repetition_dis(0, results.size());
		for (std::size_t i = 0; i != number_of_elements_with_repetitions; ++i)
		{
			results.push_back({ repetition_dis(gen), counter });
			++counter;
		}

		if (number_of_elements_with_repetitions != 0)
			std::shuffle(results.begin(), results.end(), gen);

		return results;
	}
}