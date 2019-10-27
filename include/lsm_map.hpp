#ifndef LSM_MAP_HPP
#define LSM_MAP_HPP

#pragma once

#include <lsm_base.hpp>
#include <lsm_map_parameters.hpp>
#include <lsm_unique_container.hpp>

namespace lsm
{
	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
	class map : public detail::lsm_unique_container<detail::lsm_base<detail::lsm_map_parameters<Key, T, Compare, Allocator>>>
	{
		using self_type = map<Key, T, Compare, Allocator>;
		using super_type = detail::lsm_unique_container<detail::lsm_base<detail::lsm_map_parameters<Key, T, Compare, Allocator>>>;

	public:
		using iterator = typename super_type::iterator;
		using value_type = typename super_type::value_type;

	public:
		map()
			: super_type()
		{
		}

		iterator insert(const value_type& x)
		{
			return super_type::insert(x);
		}

		void debug() const
		{
			super_type::debug();
		}
	};
}

#endif // LSM_MAP_HPP
