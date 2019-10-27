#ifndef LSM_BASE_PARAMETERS_HPP
#define LSM_BASE_PARAMETERS_HPP

#pragma once

#include <cstddef>
#include <memory>

namespace lsm
{
	namespace detail
	{
		template <class Key, class Compare, class Allocator>
		struct lsm_base_parameters
		{
			using self_type = lsm_base_parameters<Key, Compare, Allocator>;
			using key_type = Key;
			using size_type = int;// std::size_t;
			using difference_type = std::ptrdiff_t;
			using key_compare = Compare;
			using allocator_type = Allocator;
			using pointer = typename std::allocator_traits<Allocator>::pointer;
			using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
		};
	}
}

#endif // LSM_BASE_PARAMETERS_HPP
