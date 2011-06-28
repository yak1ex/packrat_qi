/***********************************************************************/
/*                                                                     */
/* test_any_key.cpp: Test code for any key holder                      */
/*                                                                     */
/*     Written by Yak! / Yasutaka ATARASHI                             */
/*                                                                     */
/*     This software is distributed under the terms of                 */
/*     Boost Software License 1.0                                      */
/*     (http://www.boost.org/LICENSE_1_0.txt).                         */
/*                                                                     */
/*     $Id$                 */
/*                                                                     */
/***********************************************************************/

#include "any_key.hpp"

#include <boost/test/auto_unit_test.hpp>

///////////////////////////////////////////////////////////////////////

yak::util::any_key ak[] = {
	std::string("abcd"),
	std::string("hijk"),
	47,
	40,
	3.5,
	1.0
};

BOOST_AUTO_TEST_CASE(test_any_key_eq)
{
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK( (ak_ == ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK( (ak[i] == ak[j]));
			} else {
				BOOST_CHECK(!(ak[i] == ak[j]));
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(test_any_key_ne)
{
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK(!(ak_ != ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK(!(ak[i] != ak[j]));
			} else {
				BOOST_CHECK( (ak[i] != ak[j]));
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(test_any_key_lt)
{
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK(!(ak_ != ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK(!(ak[i] < ak[j]));
			} else {
				BOOST_CHECK(((ak[i] < ak[j]) ^ (ak[j] < ak[i])) == 1);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(test_any_key_le)
{
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK(!(ak_ != ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK( (ak[i] <= ak[j]));
			} else {
				BOOST_CHECK(((ak[i] <= ak[j]) ^ (ak[j] <= ak[i])) == 1);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(test_any_key_gt)
{
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK(!(ak_ > ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK(!(ak[i] > ak[j]));
			} else {
				BOOST_CHECK(((ak[i] > ak[j]) ^ (ak[j] > ak[i])) == 1);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(test_any_key_ge)
{
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK( (ak_ >= ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK( (ak[i] >= ak[j]));
			} else {
				BOOST_CHECK(((ak[i] >= ak[j]) ^ (ak[j] >= ak[i])) == 1);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(test_any_key_hash)
{
	boost::hash<yak::util::any_key> hasher;
	yak::util::any_key ak_ = ak[0];

	BOOST_CHECK_EQUAL(ak_.hash_code(), ak[0].hash_code());
	BOOST_CHECK_EQUAL(hasher(ak_), hasher(ak[0]));

	for(std::size_t i = 0; i < sizeof(ak)/sizeof(ak[0]); ++i) {
		for(std::size_t j = 0; j < sizeof(ak)/sizeof(ak[0]); ++j) {
			if(i == j) {
				BOOST_CHECK_EQUAL(ak[i].hash_code(), ak[j].hash_code());
			} else {
				BOOST_WARN_NE(ak[i].hash_code(), ak[j].hash_code());
			}
		}
	}
}
