
namespace lsm
{
	namespace detail
	{
		template <class LSM>
		lsm_unique_container<LSM>::lsm_unique_container()
			: super_type()
		{
		}

		template <class LSM>
		typename lsm_unique_container<LSM>::size_type lsm_unique_container<LSM>::count(const key_type& key) const
		{
			return 1;
		}

		template <class LSM>
		typename lsm_unique_container<LSM>::iterator lsm_unique_container<LSM>::insert(const value_type& value)
		{
			auto it = find(extract_key(value));
			if (it != end())
			{
				*it = value;
				return it;
			}

			auto pair = get_buffer().insert(value);
			if (!pair.second)
			{
				get_buffer().sort();
				fractional_cascading();
				pair = get_buffer().insert(value);
				++m_active_storages;
			}
			++m_number_of_elements;
			return pair.first;
		}

		template <class LSM>
		typename lsm_unique_container<LSM>::iterator lsm_unique_container<LSM>::insert(value_type&& value)
		{
			auto it = find(value);
			if (it != end())
			{
				*it = std::move(value);
				return it;
			}

			auto pair = get_buffer().insert(std::move(value));
			if (!pair.second)
			{
				get_buffer().sort();
				fractional_cascading();
				pair = get_buffer().insert(std::move(value));
				++m_active_storages;
			}
			++m_number_of_elements;
			return pair.first;
		}

		template <class LSM>
		void lsm_unique_container<LSM>::fractional_cascading()
		{
			size_type i = 0;
			size_type number_of_elements_at_level_i = m_buffers[0].original_size();

			if (m_storages)
			{
				while (is_level_full(i))
				{
					merge(i, number_of_elements_at_level_i);
					m_current_buffer = !m_current_buffer;
					clear(i, number_of_elements_at_level_i);

					++i;
					number_of_elements_at_level_i <<= 1u;
				}
			}

			if (i >= number_of_storages())
			{
				append_storage();
				move(i, number_of_elements_at_level_i);
				extend_buffers(number_of_elements_at_level_i);
			}
			else
				move(i, number_of_elements_at_level_i);

			get_buffer(true).clear();
			get_buffer(false).clear();
		}


		template <class LSM>
		void lsm_unique_container<LSM>::clear(size_type storage_index, size_type number_of_elements)
		{
			std::destroy_n(m_storages[storage_index].get(), number_of_elements);
		}

		template <class LSM>
		void lsm_unique_container<LSM>::merge(size_type storage_index, size_type number_of_elements)
		{
			std::merge(get_storage(storage_index).get(), get_storage(storage_index).get() + number_of_elements,
				get_buffer().data(), get_buffer().data() + number_of_elements, get_buffer(false).data(),
				[this](auto&& lhs, auto&& rhs)
				{
					return key_compare{}(extract_key(lhs), extract_key(rhs));
				});
		}

		template <class LSM>
		void lsm_unique_container<LSM>::move(size_type storage_index, size_type number_of_elements)
		{
			std::move(get_buffer().data(), get_buffer().data() + number_of_elements, get_storage(storage_index).get());
			get_buffer().clear();
		}
	}
}
