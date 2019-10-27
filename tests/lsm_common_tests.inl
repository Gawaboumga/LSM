
namespace tests
{
	template <class LSM>
	void ensure_default_properties()
	{
		GIVEN("A default constructed LSM")
		{
			LSM lsm;
			WHEN("We ask for default properties")
			{
				THEN("It should no do any allocations")
				{
					REQUIRE(lsm.capacity() == 0);
					REQUIRE(lsm.size() == 0);
				}
			}
		}
	}

	template <class LSM>
	void ensure_default_accessor()
	{
		GIVEN("A default constructed LSM")
		{
			LSM lsm;
			WHEN("We ask for iterator")
			{
				THEN("Iterator should be valdi and not usefull")
				{
					REQUIRE(lsm.begin() == lsm.end());
					REQUIRE(lsm.cbegin() == lsm.cend());
				}
			}
		}
	}
}

