#ifndef LSM_MAP_PARAMETERS_HPP
#define LSM_MAP_PARAMETERS_HPP

#pragma once

#include <lsm_base_parameters.hpp>
#include <utility>

namespace lsm
{
	namespace detail
	{
		template <class Key, class T, class Compare, class Allocator>
		struct lsm_map_parameters : public lsm_base_parameters<Key, Compare, Allocator>
		{
			using super_type = lsm_base_parameters<Key, Compare, Allocator>;
			using self_type = lsm_map_parameters<Key, T, Compare, Allocator>;
			using key_type = typename super_type::key_type;
			using value_type = std::pair<Key, T>;
			using reference = value_type &;
			using const_reference = const value_type &;
			
			static const key_type& extract_key(const value_type& value)
			{
				return value.first;
			}
		};
	}
}

#endif // LSM_MAP_PARAMETERS_HPP
