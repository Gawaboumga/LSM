#include "lsm_map.hpp"
#include <Catch2/catch.hpp>

#include "lsm_multiple_tests.hpp"

namespace tests
{
	using my_map = lsm::map<int, int>;

	template <>
	struct convert_random_to_value_type<my_map>
	{
		typename my_map::value_type operator()(const randomly_generated_type& pair)
		{
			return pair;
		}
	};

	template <>
	struct compare_iterator_to_random<my_map>
	{
		bool operator()(my_map::iterator it, const randomly_generated_type& pair)
		{
			return it->first == pair.first && it->second == pair.second;
		}

		bool operator()(my_map::const_iterator it, const randomly_generated_type& pair)
		{
			return it->first == pair.first && it->second == pair.second;
		}
	};
}

TEST_CASE("LSM map", "[LSM][MAP]")
{
	SECTION("Default constructed LSM")
	{
		using lsm_type = lsm::map<int, int>;

		tests::ensure_default_properties<lsm_type>();
		tests::ensure_default_accessor<lsm_type>();
	}

	SECTION("Default constructions and insertions without duplicates")
	{
		using lsm_type = lsm::map<int, int>;

		tests::ensure_default_insertion_without_duplicates<lsm_type>();
	}

	SECTION("Default constructions and find without duplicates")
	{
		using lsm_type = lsm::map<int, int>;

		tests::ensure_default_find_without_duplicates<lsm_type>();
	}

	SECTION("Default constructions and lower bound without duplicates")
	{
		using lsm_type = lsm::map<int, int>;

		tests::ensure_default_lower_bound_without_duplicates<lsm_type>();
		tests::ensure_default_lower_bound_without_duplicates_other_range<lsm_type>();
	}

	SECTION("Default constructions and upper bound without duplicates")
	{
		using lsm_type = lsm::map<int, int>;

		tests::ensure_default_upper_bound_without_duplicates<lsm_type>();
		tests::ensure_default_upper_bound_without_duplicates_other_range<lsm_type>();
	}

	SECTION("Count should take into account multiplicity")
	{
		using lsm_type = lsm::map<int, int>;

		tests::ensure_default_count_without_duplicates<lsm_type>();
		tests::ensure_default_count_with_duplicates<lsm_type>();
	}
}