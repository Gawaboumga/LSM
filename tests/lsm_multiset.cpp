#include "lsm_multiset.hpp"
#include <Catch2/catch.hpp>

#include "lsm_multiple_tests.hpp"

namespace tests
{
	using my_multi_set = lsm::multi_set<int>;

	template <>
	struct convert_random_to_value_type<my_multi_set>
	{
		typename my_multi_set::value_type operator()(const randomly_generated_type& pair)
		{
			return pair.first;
		}
	};

	template <>
	struct compare_iterator_to_random<my_multi_set>
	{
		bool operator()(my_multi_set::iterator it, const randomly_generated_type& pair)
		{
			return *it == pair.first;
		}

		bool operator()(my_multi_set::const_iterator it, const randomly_generated_type& pair)
		{
			return *it == pair.first;
		}
	};
}

TEST_CASE("LSM multiset", "[LSM][MULTISET]")
{
	SECTION("Default constructed LSM")
	{
		using lsm_type = lsm::multi_set<int>;

		tests::ensure_default_properties<lsm_type>();
		tests::ensure_default_accessor<lsm_type>();
	}

	SECTION("Default constructions and insertions without duplicates")
	{
		using lsm_type = lsm::multi_set<int>;

		tests::ensure_default_insertion_without_duplicates<lsm_type>();
	}

	SECTION("Default constructions and find without duplicates")
	{
		using lsm_type = lsm::multi_set<int>;

		tests::ensure_default_find_without_duplicates<lsm_type>();
	}

	SECTION("Default constructions and lower bound without duplicates")
	{
		using lsm_type = lsm::multi_set<int>;

		tests::ensure_default_lower_bound_without_duplicates<lsm_type>();
	}

	SECTION("Default constructions and upper bound without duplicates")
	{
		using lsm_type = lsm::multi_set<int>;

		tests::ensure_default_upper_bound_without_duplicates<lsm_type>();
	}
}
