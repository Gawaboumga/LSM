#ifndef LSM_SET_PARAMETERS_HPP
#define LSM_SET_PARAMETERS_HPP

#pragma once

#include <lsm_base_parameters.hpp>
#include <utility>

namespace lsm
{
	namespace detail
	{
		template <class Key, class Compare, class Allocator>
		struct lsm_set_parameters : public lsm_base_parameters<Key, Compare, Allocator>
		{
			using self_type = lsm_set_parameters<Key, Compare, Allocator>;
			using value_type = Key;
			using reference = value_type &;
			using const_reference = const value_type &;

			static const key_type& extract_key(const value_type& value)
			{
				return value;
			}
		};
	}
}

#endif // LSM_SET_PARAMETERS_HPP
