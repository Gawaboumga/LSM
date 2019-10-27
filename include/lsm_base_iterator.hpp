#ifndef LSM_BASE_ITERATOR_HPP
#define LSM_BASE_ITERATOR_HPP

#pragma once

#include <iterator>

namespace lsm
{
	namespace detail
	{
		template <class Parameters>
		struct lsm_base_iterator
		{
		public:
			using value_type = typename Parameters::value_type;
			using pointer    = value_type*;
			using reference  = value_type &;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			lsm_base_iterator() : 
				m_ptr(nullptr)
			{
			}

			lsm_base_iterator(value_type* ptr) :
				m_ptr(ptr)
			{
			}

			reference operator*()
			{
				return *m_ptr;
			}

			const reference operator*() const
			{
				return *m_ptr;
			}

			pointer operator->()
			{
				return m_ptr;
			}

			const pointer operator->() const
			{
				return m_ptr;
			}

			lsm_base_iterator& operator++()
			{
				++m_ptr;
				return *this;
			}

			lsm_base_iterator& operator--()
			{
				--m_ptr;
				return *this;
			}

			lsm_base_iterator operator++(int)
			{
				lsm_base_iterator r(*this);
				++m_ptr;
				return r;
			}

			lsm_base_iterator operator--(int)
			{
				lsm_base_iterator r(*this);
				--m_ptr;
				return r;
			}

			bool operator!=(const lsm_base_iterator& r) const
			{
				return m_ptr != r.m_ptr;
			}

			bool operator==(const lsm_base_iterator& r) const
			{
				return m_ptr == r.m_ptr;
			}

		private:
			pointer m_ptr;
		};

		template <class Parameters>
		struct lsm_base_const_iterator
		{
		public:
			using value_type = const typename Parameters::value_type;
			using pointer    = value_type *;
			using reference  = value_type &;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			lsm_base_const_iterator() :
				m_ptr(nullptr)
			{
			}

			lsm_base_const_iterator(value_type* ptr) :
				m_ptr(ptr)
			{
			}

			reference operator*()
			{
				return *m_ptr;
			}

			const reference operator*() const
			{
				return *m_ptr;
			}

			pointer operator->()
			{
				return m_ptr;
			}

			const pointer operator->() const
			{
				return m_ptr;
			}

			lsm_base_const_iterator& operator++()
			{
				++m_ptr;
				return *this;
			}

			lsm_base_const_iterator& operator--()
			{
				--m_ptr;
				return *this;
			}

			lsm_base_const_iterator operator++(int)
			{
				lsm_base_iterator r(*this);
				++m_ptr;
				return r;
			}

			lsm_base_const_iterator operator--(int)
			{
				lsm_base_iterator r(*this);
				--m_ptr;
				return r;
			}

			bool operator!=(const lsm_base_const_iterator& r) const
			{
				return m_ptr != r.m_ptr;
			}

			bool operator==(const lsm_base_const_iterator& r) const
			{
				return m_ptr == r.m_ptr;
			}

		private:
			pointer m_ptr;
		};
	}
}

#endif // LSM_BASE_ITERATOR_HPP
