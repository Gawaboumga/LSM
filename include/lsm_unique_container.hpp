#ifndef LSM_UNIQUECONTAINER_HPP
#define LSM_UNIQUECONTAINER_HPP

#pragma once

#include <lsm_container.hpp>

namespace lsm
{
	namespace detail
	{
		template <class LSM>
		struct lsm_unique_container : public lsm_container<LSM>
		{
		public:
			using super_type = lsm_container<LSM>;
			using key_type = typename LSM::key_type;
			using value_type = typename LSM::value_type;
			using size_type = typename LSM::size_type;
			using allocator_type = typename LSM::allocator_type;
			using iterator = typename LSM::iterator;
			using const_iterator = typename LSM::const_iterator;

			lsm_unique_container();

		public:
			size_type count(const key_type& key) const;

			iterator insert(const value_type& value);
			iterator insert(value_type&& value);

		private:
			void fractional_cascading();

			void clear(size_type storage_index, size_type number_of_elements);

			void merge(size_type storage_index, size_type number_of_elements);
			void move(size_type storage_index, size_type number_of_elements);
		};
	}
}

#include <lsm_unique_container.inl>

#endif // LSM_UNIQUECONTAINER_HPP
