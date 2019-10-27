#ifndef LSM_BASE_HPP
#define LSM_BASE_HPP

#pragma once

#include <lsm_base_iterator.hpp>

namespace lsm
{
	namespace detail
	{
		template <class Parameters>
		struct lsm_base
		{
			using key_type = typename Parameters::key_type;
			using value_type = typename Parameters::value_type;
			using size_type = typename Parameters::size_type;
			using difference_type = typename Parameters::difference_type;
			using key_compare = typename Parameters::key_compare;
			using allocator_type = typename Parameters::allocator_type;
			using reference = typename Parameters::reference;
			using const_reference = typename Parameters::const_reference;
			using pointer = typename Parameters::pointer;
			using const_pointer = typename Parameters::const_pointer;

			using iterator = lsm_base_iterator<Parameters>;
			using const_iterator = lsm_base_const_iterator<Parameters>;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			static const key_type& extract_key(const value_type& value)
			{
				return Parameters::extract_key(value);
			}
		};
	}
}

#endif // LSM_BASE_HPP
