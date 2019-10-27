#ifndef LSM_COMMON_HPP
#define LSM_COMMON_HPP

#include "utils.hpp"

namespace tests
{
	template <class LSM>
	struct convert_random_to_value_type
	{
		typename LSM::value_type operator()(const randomly_generated_type& pair)
		{
		}
	};

	template <class LSM>
	struct compare_iterator_to_random
	{
		bool operator()(typename LSM::iterator it, const randomly_generated_type& pair)
		{
		}
	};

	template <class LSM>
	struct compare_values
	{
		bool operator()(typename LSM::value_type lhs, typename LSM::value_type rhs)
		{
			return lhs == rhs;
		}
	};

	template <class LSM>
	void ensure_default_properties();

	template <class LSM>
	void ensure_default_accessor();
}

#include "lsm_common_tests.inl"

#endif // LSM_COMMON_HPP
