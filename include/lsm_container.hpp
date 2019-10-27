#ifndef LSM_CONTAINER_HPP
#define LSM_CONTAINER_HPP

#pragma once

#include <vector>
#include <utility>

namespace lsm
{
	namespace detail
	{
		template <class LSM>
		class lsm_container
		{
			template <std::size_t N = 2>
			class buffer
			{
			public:
				using size_type = typename LSM::size_type;
				using key_type = typename LSM::key_type;
				using value_type = typename LSM::value_type;
				using size_type = typename LSM::size_type;
				using iterator = typename LSM::iterator;
				using const_iterator = typename LSM::const_iterator;

				buffer() :
					m_buffer()
				{
					m_buffer.reserve(N);
				}

				value_type* data()
				{
					return m_buffer.data();
				}

				const value_type* data() const
				{
					return m_buffer.data();
				}

				size_type count(const key_type& key) const
				{
					return std::count_if(m_buffer.begin(), m_buffer.end(), [&key](const value_type& lhs)
						{
							return extract_key(lhs) == key;
						});
				}

				void clear()
				{
					return m_buffer.clear();
				}

				void debug() const
				{
					printf("Buffer: size %d, capacity %d\n", m_buffer.size(), m_buffer.capacity());
					for (auto&& e : m_buffer)
						printf("{%d, %d}\n", e.first, e.second);
				}

				iterator find(const key_type& key)
				{
					auto first = m_buffer.begin();
					auto last = m_buffer.end();
					first = std::find_if(first, last, [&key](const value_type& lhs)
						{
							return extract_key(lhs) == key;
						});
					if (first == last)
						return nullptr;
					return &*first;
				}

				const_iterator find(const key_type& key) const
				{
					auto first = m_buffer.begin();
					auto last = m_buffer.end();
					first = std::find_if(first, last, [&key](const value_type& lhs)
						{
							return extract_key(lhs) == key;
						});
					if (first == last)
						return nullptr;
					return &*first;
				}

				std::pair<iterator, bool> insert(const value_type& value)
				{
					if (m_buffer.size() >= original_size())
						return std::make_pair<iterator, bool>(nullptr, false);
					auto& ref = m_buffer.emplace_back(value);
					return std::make_pair<iterator, bool>(&ref, true);
				}

				std::pair<iterator, bool> insert(value_type&& value)
				{
					if (m_buffer.size() >= original_size())
						return std::make_pair(nullptr, false);
					auto& ref = m_buffer.emplace_back(std::move(value));
					return std::make_pair<iterator, bool>(&ref, true);
				}

				iterator lower_bound(const key_type& key)
				{
					auto first = m_buffer.begin();
					auto last = m_buffer.end();
					auto best_it = m_buffer.end();
					for (auto it = first; it != last; ++it)
					{
						if (!key_compare{}(extract_key(*it), key))
						{
							if (best_it == m_buffer.end())
								best_it = it;
							else if (key_compare{}(extract_key(*it), extract_key(*best_it)))
								best_it = it;
						}
					}
					if (best_it == m_buffer.end())
						return nullptr;
					else
						return &*best_it;
				}

				const_iterator lower_bound(const key_type& key) const
				{
					auto first = m_buffer.begin();
					auto last = m_buffer.end();
					auto best_it = m_buffer.end();
					for (auto it = first; it != last; ++it)
					{
						if (!key_compare{}(extract_key(*it), key))
						{
							if (best_it == m_buffer.end())
								best_it = it;
							else if (key_compare{}(extract_key(*it), extract_key(*best_it)))
								best_it = it;
						}
					}
					if (best_it == m_buffer.end())
						return nullptr;
					else
						return &*best_it;
				}

				size_type size() const
				{
					return m_buffer.size();
				}

				void sort()
				{
					std::sort(m_buffer.begin(), m_buffer.end());
				}

				void reserve(size_type capacity)
				{
					m_buffer.reserve(capacity);
				}

				bool remove_duplicates()
				{
					m_buffer.erase(std::unique(m_buffer.begin(), m_buffer.end(), m_buffer.end()));
				}

				iterator upper_bound(const key_type& key)
				{
					auto first = m_buffer.begin();
					auto last = m_buffer.end();
					auto best_it = m_buffer.end();
					for (auto it = first; it != last; ++it)
					{
						if (!key_compare{}(extract_key(*it), key))
						{
							if (best_it == m_buffer.end())
								best_it = it;
							else if (key_compare{}(extract_key(*it), extract_key(*best_it)))
								best_it = it;
						}
					}
					if (best_it == m_buffer.end())
						return nullptr;
					else
						return &*best_it;
				}

				const_iterator upper_bound(const key_type& key) const
				{
					auto first = m_buffer.begin();
					auto last = m_buffer.end();
					auto best_it = m_buffer.end();
					for (auto it = first; it != last; ++it)
					{
						if (key_compare{}(key, extract_key(*it)) && !key_compare{}(extract_key(*it), key))
						{
							if (best_it == m_buffer.end())
								best_it = it;
							else if (key_compare{}(extract_key(*it), extract_key(*best_it)))
								best_it = it;
						}
					}

					if (best_it == m_buffer.end())
						return nullptr;
					else
						return &*best_it;
				}

			public:
				size_type original_power_of_two() const
				{
					return N;
				}

				size_type original_size() const
				{
					return size_type(1) << N;
				}
			private:
				std::vector<value_type> m_buffer;
			};


		protected:
			using size_type = typename LSM::size_type;
			using key_type = typename LSM::key_type;
			using value_type = typename LSM::value_type;
			using pointer = typename LSM::pointer;
			using const_pointer = typename LSM::const_pointer;
			using iterator = typename LSM::iterator;
			using const_iterator = typename LSM::const_iterator;
			using reverse_iterator = typename LSM::reverse_iterator;
			using const_reverse_iterator = typename LSM::const_reverse_iterator;
			using key_compare = typename LSM::key_compare;
			using allocator_type = typename LSM::allocator_type;
			using storage_type = std::unique_ptr<value_type[]>;

		public:
			lsm_container();

			iterator begin() noexcept;
			const_iterator begin() const noexcept;
			const_iterator cbegin() const noexcept;

			iterator end() noexcept;
			const_iterator end() const noexcept;
			const_iterator cend() const noexcept;

			reverse_iterator rbegin() noexcept;
			const_reverse_iterator rbegin() const noexcept;
			const_reverse_iterator crbegin() const noexcept;

			reverse_iterator rend() noexcept;
			const_reverse_iterator rend() const noexcept;
			const_reverse_iterator crend() const noexcept;

			size_type buffer_size() const;

			size_type capacity() const;
			void clear() noexcept;
			bool contains(const key_type& key) const;

			bool empty() const noexcept;
			std::pair<iterator, iterator> equal_range(const key_type& key);
			std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

			iterator find(const key_type& key);
			const_iterator find(const key_type& key) const;

			allocator_type get_allocator() const;

			iterator lower_bound(const key_type& key);
			const_iterator lower_bound(const key_type& key) const;

			size_type size() const;

			iterator upper_bound(const key_type& key);
			const_iterator upper_bound(const key_type& key) const;

		protected:
			void append_storage();

			iterator convert_to_non_const_iterator(const_iterator cit) const;

			void extend_buffers(size_type capacity);
			static const key_type& extract_key(const value_type& value);

			struct for_each_storage_parameter_type
			{
				for_each_storage_parameter_type(value_type* values, size_type index, size_type number_of_elements, bool active) :
					values(values),
					index(index),
					number_of_elements(number_of_elements),
					active(active)
				{
				}

				value_type* values;
				size_type index;
				size_type number_of_elements;
				bool active;

				value_type* data() const
				{
					return values;
				}

				bool is_active() const
				{
					return active;
				}
			};

			for_each_storage_parameter_type storage_iterator() const;
			bool next_storage(for_each_storage_parameter_type& storage) const;

			buffer<>& get_buffer(bool active = true);
			const buffer<>& get_buffer(bool active = true) const;
			std::unique_ptr<value_type[]>& get_storage(size_type storage_index);
			const std::unique_ptr<value_type[]>& get_storage(size_type storage_index) const;

			bool is_level_full(size_type storage_index) const;

			size_type number_of_storages() const;

			void debug() const;

		protected:

			std::unique_ptr<storage_type[]> m_storages;
			size_type m_active_storages;
			buffer<> m_buffers[2];
			size_type m_number_of_elements;
			bool m_current_buffer;
		};
	}
}

#include <lsm_container.inl>

#endif // LSM_CONTAINER_HPP
