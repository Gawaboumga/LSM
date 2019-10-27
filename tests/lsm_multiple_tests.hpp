#ifndef LSM_MULTIPLE_HPP
#define LSM_MULTIPLE_HPP

#pragma once

#include "lsm_common_tests.hpp"

namespace tests
{
	template <class LSM>
	void ensure_default_insertion_without_duplicates();

	template <class LSM>
	void ensure_default_find_without_duplicates();
}

#include "lsm_multiple_tests.inl"

#endif // LSM_MULTIPLE_HPP
