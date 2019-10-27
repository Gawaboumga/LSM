#ifndef LSM_MULTISET_HPP
#define LSM_MULTISET_HPP

#pragma once

#include <lsm_base.hpp>
#include <lsm_set_parameters.hpp>
#include <lsm_multi_container.hpp>

namespace lsm
{
	template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
	class multi_set : public detail::lsm_multi_container<detail::lsm_base<detail::lsm_set_parameters<Key, Compare, Allocator>>>
	{
		using self_type = multi_set<Key, Compare, Allocator>;
		using super_type = detail::lsm_multi_container<detail::lsm_base<detail::lsm_set_parameters<Key, Compare, Allocator>>>;

	public:
		using iterator = typename super_type::iterator;
		using value_type = typename super_type::value_type;

	public:
		multi_set()
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

#endif // LSM_MULTISET_HPP
