
namespace lsm
{
	namespace detail
	{
		namespace fls
		{
			inline std::size_t biggest_power_of_two(std::size_t num)
			{
				if (!num)
					return 0;

				std::size_t ret = 1;
				while (num >>= 1)
					ret <<= 1;

				return ret;
			}

			inline std::size_t index_of_lsb(std::size_t num)
			{
				if (!num)
					return 0;

				std::size_t ret = 1;
				while (num >>= 1)
					++ret;

				return ret;
			}
		}

		template <class LSM>
		lsm_container<LSM>::lsm_container() :
			m_storages(nullptr),
			m_active_storages(0),
			m_buffers(),
			m_number_of_elements(0),
			m_current_buffer(false)
		{
		}

		template <class LSM>
		typename lsm_container<LSM>::iterator lsm_container<LSM>::begin() noexcept
		{
			if (get_buffer().size() != 0)
				return get_buffer().data();

			auto storage = storage_iterator();
			while (next_storage(storage))
			{
				if (storage.is_active())
					return storage.data();
			}

			return nullptr;
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::begin() const noexcept
		{
			if (get_buffer().size() != 0)
				return const_iterator(get_buffer().data());

			auto storage = storage_iterator();
			while (next_storage(storage))
			{
				if (storage.is_active())
					return storage.data();
			}

			return nullptr;
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::cbegin() const noexcept
		{
			return begin();
		}

		template <class LSM>
		typename lsm_container<LSM>::iterator lsm_container<LSM>::end() noexcept
		{
			return nullptr;
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::end() const noexcept
		{
			return nullptr;
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::cend() const noexcept
		{
			return end();
		}

		template <class LSM>
		typename lsm_container<LSM>::reverse_iterator lsm_container<LSM>::rbegin() noexcept
		{
			return reverse_iterator(end());
		}

		template <class LSM>
		typename lsm_container<LSM>::const_reverse_iterator lsm_container<LSM>::rbegin() const noexcept
		{
			return reverse_iterator(end());
		}

		template <class LSM>
		typename lsm_container<LSM>::const_reverse_iterator lsm_container<LSM>::crbegin() const noexcept
		{
			return reverse_iterator(cend());
		}

		template <class LSM>
		typename lsm_container<LSM>::reverse_iterator lsm_container<LSM>::rend() noexcept
		{
			return reverse_iterator(begin());
		}

		template <class LSM>
		typename lsm_container<LSM>::const_reverse_iterator lsm_container<LSM>::rend() const noexcept
		{
			return reverse_iterator(begin());
		}

		template <class LSM>
		typename lsm_container<LSM>::const_reverse_iterator lsm_container<LSM>::crend() const noexcept
		{
			return reverse_iterator(cbegin());
		}

		template <class LSM>
		typename lsm_container<LSM>::size_type lsm_container<LSM>::buffer_size() const
		{
			return get_buffer().original_size();
		}

		template <class LSM>
		typename lsm_container<LSM>::size_type lsm_container<LSM>::capacity() const
		{
			auto n = fls::biggest_power_of_two(m_active_storages);
			return n * get_buffer().original_size();
		}

		template <class LSM>
		void lsm_container<LSM>::clear() noexcept
		{
			get_buffer(true).clear();
			get_buffer(false).clear();
			m_number_of_elements = 0;
		}

		template <class LSM>
		bool lsm_container<LSM>::contains(const key_type& key) const
		{
			return find(key) != end();
		}

		template <class LSM>
		bool lsm_container<LSM>::empty() const noexcept
		{
			return m_number_of_elements == 0;
		}

		template <class LSM>
		std::pair<typename lsm_container<LSM>::iterator, typename lsm_container<LSM>::iterator> lsm_container<LSM>::equal_range(const key_type& key)
		{

		}

		template <class LSM>
		std::pair<typename lsm_container<LSM>::const_iterator, typename lsm_container<LSM>::const_iterator> lsm_container<LSM>::equal_range(const key_type& key) const
		{

		}

		template <class LSM>
		typename lsm_container<LSM>::iterator lsm_container<LSM>::find(const key_type& key)
		{
			return convert_to_non_const_iterator(std::as_const(*this).find(key));
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::find(const key_type& key) const
		{
			auto storage = storage_iterator();
			while (next_storage(storage))
			{
				if (storage.is_active())
				{
					auto first = storage.data();
					auto last = storage.data() + storage.number_of_elements;
					first = std::lower_bound(first, last, key, [this](const value_type& lhs, const key_type& rhs_key)
						{
							const key_type& lhs_key = extract_key(lhs);
							//const key_type& rhs_key = extract_key(rhs);
							return key_compare{}(lhs_key, rhs_key);
						});

					if (first == last)
						continue;

					const value_type& first_value = *first;
					if (!(key_compare{}(key, extract_key(first_value))))
						return first;
				}
			}

			return get_buffer().find(key);
		}

		template <class LSM>
		typename lsm_container<LSM>::allocator_type lsm_container<LSM>::get_allocator() const
		{
			return allocator_type{};
		}

		template <class LSM>
		typename lsm_container<LSM>::iterator lsm_container<LSM>::lower_bound(const key_type& key)
		{
			return convert_to_non_const_iterator(std::as_const(*this).lower_bound(key));
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::lower_bound(const key_type& key) const
		{
			auto storage = storage_iterator();
			auto best_it = end();
			while (next_storage(storage))
			{
				if (storage.is_active())
				{
					auto first = storage.data();
					auto last = storage.data() + storage.number_of_elements;
					first = std::lower_bound(first, last, key, [this](const value_type& lhs, const key_type& rhs_key)
						{
							const key_type& lhs_key = extract_key(lhs);
							return key_compare{}(lhs_key, rhs_key);
						});

					if (!(first == last))
					{
						const value_type& first_value = *first;
						if (!key_compare{}(key, extract_key(first_value)))
							return first;

						if (best_it == end())
							best_it = first;
						else if (key_compare{}(extract_key(*first), extract_key(*best_it)))
							best_it = first;
					}
				}
			}

			auto buffer_it = get_buffer().lower_bound(key);
			if (best_it == end())
				return buffer_it;
			else if (buffer_it == end())
				return best_it;
			else if (key_compare{}(extract_key(*buffer_it), extract_key(*best_it)))
				return buffer_it;
			else
				return best_it;
		}

		template <class LSM>
		typename lsm_container<LSM>::size_type lsm_container<LSM>::size() const
		{
			return m_number_of_elements;
		}

		template <class LSM>
		typename lsm_container<LSM>::iterator lsm_container<LSM>::upper_bound(const key_type& key)
		{
			return convert_to_non_const_iterator(std::as_const(*this).upper_bound(key));
		}

		template <class LSM>
		typename lsm_container<LSM>::const_iterator lsm_container<LSM>::upper_bound(const key_type& key) const
		{
			auto storage = storage_iterator();
			auto best_it = end();
			while (next_storage(storage))
			{
				if (storage.is_active())
				{
					auto first = storage.data();
					auto last = storage.data() + storage.number_of_elements;
					first = std::upper_bound(first, last, key, [this](const key_type& lhs_key, const value_type& rhs)
						{
							const key_type& rhs_key = extract_key(rhs);
							return key_compare{}(lhs_key, rhs_key);
						});
					
					if (!(first == last))
					{
						const value_type& first_value = *first;
						if (!key_compare{}(key, extract_key(first_value)))
							return first;

						if (best_it == end())
							best_it = first;
						else if (!key_compare{}(extract_key(*best_it), extract_key(*first)))
							best_it = first;
					}
				}
			}

			auto buffer_it = get_buffer().upper_bound(key);
			if (best_it == end())
				return buffer_it;
			else if (buffer_it == end())
				return best_it;
			else if (key_compare{}(extract_key(*buffer_it), extract_key(*best_it)))
				return buffer_it;
			else
				return best_it;
		}

		template <class LSM>
		void lsm_container<LSM>::append_storage()
		{
			auto previous_number_of_storages = number_of_storages();
			auto original_power_of_two = get_buffer().original_power_of_two();

			auto tmp = std::make_unique<std::unique_ptr<value_type[]>[]>(previous_number_of_storages + size_type(1));
			tmp[previous_number_of_storages] = std::make_unique<value_type[]>(size_type(1) << (previous_number_of_storages + original_power_of_two));

			for (size_type i = 0; i != previous_number_of_storages; ++i)
				tmp[i] = std::move(m_storages[i]);

			m_storages = std::move(tmp);
		}

		template <class LSM>
		typename lsm_container<LSM>::iterator lsm_container<LSM>::convert_to_non_const_iterator(typename lsm_container<LSM>::const_iterator cit) const
		{
			return const_cast<typename iterator::value_type*>(&*cit);
		}

		template <class LSM>
		void lsm_container<LSM>::extend_buffers(size_type capacity)
		{
			get_buffer(true).reserve(capacity * 2);
			get_buffer(false).reserve(capacity * 2);
		}

		template <class LSM>
		const typename lsm_container<LSM>::key_type& lsm_container<LSM>::extract_key(const value_type& value)
		{
			return LSM::extract_key(value);
		}

		template <class LSM>
		typename lsm_container<LSM>::for_each_storage_parameter_type lsm_container<LSM>::storage_iterator() const
		{
			for_each_storage_parameter_type result(
				nullptr,
				-1,
				0,
				false
			);
			return result;
		}

		template <class LSM>
		bool lsm_container<LSM>::next_storage(for_each_storage_parameter_type& storage) const
		{
			if (!m_active_storages)
				return false;

			if (storage.index == -1)
			{
				storage.values = m_storages[0].get();
				storage.index = 0;
				storage.number_of_elements = get_buffer().original_size();
				storage.active = 1 & m_active_storages;
				return true;
			}

			auto n = fls::biggest_power_of_two(m_active_storages);
			auto new_index = storage.index + 1;
			auto i = 1 << storage.index;

			if (i >= n)
				return false;

			storage.values = m_storages[new_index].get();
			storage.index = new_index;
			storage.number_of_elements = storage.number_of_elements << 1;
			storage.active = (1 << new_index) & m_active_storages;
			return true;
		}

		template <class LSM>
		typename lsm_container<LSM>::buffer<>& lsm_container<LSM>::get_buffer(bool active)
		{
			if (active)
				return m_buffers[m_current_buffer];
			else
				return m_buffers[!m_current_buffer];
		}

		template <class LSM>
		const typename lsm_container<LSM>::buffer<>& lsm_container<LSM>::get_buffer(bool active) const
		{
			if (active)
				return m_buffers[m_current_buffer];
			else
				return m_buffers[!m_current_buffer];
		}

		template <class LSM>
		std::unique_ptr<typename lsm_container<LSM>::value_type[]>& lsm_container<LSM>::get_storage(size_type storage_index)
		{
			return m_storages[storage_index];
		}

		template <class LSM>
		const std::unique_ptr<typename lsm_container<LSM>::value_type[]>& lsm_container<LSM>::get_storage(size_type storage_index) const
		{
			return m_storages[storage_index];
		}

		template <class LSM>
		bool lsm_container<LSM>::is_level_full(size_type storage_index) const
		{
			return m_active_storages & (1 << storage_index);
		}

		template <class LSM>
		typename lsm_container<LSM>::size_type lsm_container<LSM>::number_of_storages() const
		{
			return fls::index_of_lsb(m_active_storages);
		}

		template <class LSM>
		void lsm_container<LSM>::debug() const
		{
			printf("size: %d\n", m_number_of_elements);
			printf("Active: %d\t", m_active_storages);
			auto n = m_active_storages;
			while (n) {
				if (n & 1)
					printf("1");
				else
					printf("0");

				n >>= 1;
			}
			printf("\n");

			auto storage = storage_iterator();
			while (next_storage(storage))
			{
				printf("Storage %d fo size: %d\n", storage.index, storage.number_of_elements);
				if (storage.is_active())
				{
					for (int j = 0; j != storage.number_of_elements; ++j)
						printf("{%d, %d}", storage.data()[j].first, storage.data()[j].second);
					printf("\n");
				}
			};

			printf("Active buffer: %d\n", m_current_buffer);
			m_buffers[0].debug();
			m_buffers[1].debug();
			printf("\n\n");
		}
	}
}
