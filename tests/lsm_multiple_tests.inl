
namespace tests
{
	template <class LSM>
	void ensure_default_insertion_without_duplicates()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		for (auto&& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));
		}
	}

	template <class LSM>
	void ensure_default_find_without_duplicates()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		for (auto&& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			it = lsm.find(i.first);
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));
		}

		const LSM& const_lsm = lsm;

		for (auto&& i : elements)
		{
			auto it = lsm.find(i.first);
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			auto cit = const_lsm.find(i.first);
			REQUIRE(compare_iterator_to_random<LSM>{}(cit, i));
		}
	}

	template <class LSM>
	void ensure_default_lower_bound_without_duplicates()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		for (auto&& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			it = lsm.lower_bound(i.first);
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));
		}

		const LSM& const_lsm = lsm;

		for (auto&& i : elements)
		{
			auto it = lsm.lower_bound(i.first);
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			auto cit = const_lsm.lower_bound(i.first);
			REQUIRE(compare_iterator_to_random<LSM>{}(cit, i));
		}
	}

	template <class LSM>
	void ensure_default_lower_bound_without_duplicates_other_range()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		auto other_elements = generate_random_elements(buffer_size * 8, 0.f);
		std::sort(other_elements.begin(), other_elements.end());

		std::vector<randomly_generated_type> sorted_elements;
		auto idx = 0;
		for (const auto& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			sorted_elements.insert(std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; }), i);

			auto theoretical_it = std::lower_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs.first < rhs; });
			it = lsm.lower_bound(i.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));

			const auto& other_element = other_elements[idx];
			theoretical_it = std::lower_bound(sorted_elements.begin(), sorted_elements.end(), other_element.first, [](auto lhs, auto rhs) { return lhs.first < rhs; });
			it = lsm.lower_bound(other_element.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));

			++idx;
		}

		const LSM& const_lsm = lsm;

		idx = 0;
		for (auto&& i : elements)
		{
			auto theoretical_it = std::lower_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs.first < rhs; });
			auto it = lsm.lower_bound(i.first);
			auto cit = const_lsm.lower_bound(i.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
			{
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * cit));
			}

			const auto& other_element = other_elements[idx];
			theoretical_it = std::lower_bound(sorted_elements.begin(), sorted_elements.end(), other_element.first, [](auto lhs, auto rhs) { return lhs.first < rhs; });
			it = lsm.lower_bound(other_element.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));

			++idx;
		}
	}

	template <class LSM>
	void ensure_default_upper_bound_without_duplicates()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);

		std::vector<randomly_generated_type> sorted_elements;
		for (const auto& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			sorted_elements.insert(std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; }), i);

			auto theoretical_it = std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; });
			it = lsm.upper_bound(i.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), *it));
		}

		const LSM& const_lsm = lsm;

		for (auto&& i : elements)
		{
			auto theoretical_it = std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; });
			auto it = lsm.upper_bound(i.first);
			auto cit = const_lsm.upper_bound(i.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
			{
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), *it));
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), *cit));
			}
		}
	}

	template <class LSM>
	void ensure_default_upper_bound_without_duplicates_other_range()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		auto other_elements = generate_random_elements(buffer_size * 8, 0.f);
		std::sort(other_elements.begin(), other_elements.end());

		std::vector<randomly_generated_type> sorted_elements;
		auto idx = 0;
		for (const auto& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));

			sorted_elements.insert(std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; }), i);

			auto theoretical_it = std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; });
			it = lsm.upper_bound(i.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));

			const auto& other_element = other_elements[idx];
			theoretical_it = std::upper_bound(sorted_elements.begin(), sorted_elements.end(), other_element.first, [](auto lhs, auto rhs) { return lhs < rhs.first; });
			it = lsm.upper_bound(other_element.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));

			++idx;
		}

		const LSM& const_lsm = lsm;

		idx = 0;
		for (auto&& i : elements)
		{
			auto theoretical_it = std::upper_bound(sorted_elements.begin(), sorted_elements.end(), i.first, [](auto lhs, auto rhs) { return lhs < rhs.first; });
			auto it = lsm.upper_bound(i.first);
			auto cit = const_lsm.upper_bound(i.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
			{
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * cit));
			}

			const auto& other_element = other_elements[idx];
			theoretical_it = std::upper_bound(sorted_elements.begin(), sorted_elements.end(), other_element.first, [](auto lhs, auto rhs) { return lhs < rhs.first; });
			it = lsm.upper_bound(other_element.first);

			if (theoretical_it == sorted_elements.end())
				REQUIRE(it == lsm.end());
			else
				REQUIRE(compare_values<LSM>{}(convert_random_to_value_type<LSM>{}(*theoretical_it), * it));

			++idx;
		}
	}

	template <class LSM>
	void ensure_default_count_without_duplicates()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		for (auto&& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));
		}

		for (auto&& i : elements)
		{
			REQUIRE(lsm.count(i.first) == 1);
		}
	}

	template <class LSM>
	void ensure_default_count_with_duplicates()
	{
		LSM lsm;
		auto buffer_size = lsm.buffer_size();
		auto elements = generate_random_elements(buffer_size * 8, 0.f);
		for (auto&& i : elements)
		{
			auto it = lsm.insert(convert_random_to_value_type<LSM>{}(i));
			REQUIRE(compare_iterator_to_random<LSM>{}(it, i));
		}

		for (auto&& i : elements)
		{
			REQUIRE(lsm.count(i.first) == 1);
		}
	}
}
