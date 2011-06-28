/***********************************************************************/
/*                                                                     */
/* test_packrat_binary.cpp: Test code for Packrat parser - bin parser  */
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

#include "test_packrat_helper.hpp"
#include "packrat.hpp"
#include "packrat_rule_debug.hpp"

#include <string>

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_binary.hpp>

#include <boost/test/auto_unit_test.hpp>

bool is_le()
{
	int n = 0x01020304;
	return *static_cast<char*>(static_cast<void*>(&n)) == '\x04';
}

BOOST_AUTO_TEST_CASE(test_nativebin_parsers)
{
	namespace qi = boost::spirit::qi;

	if(is_le()) {
		std::string s = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
		std::string::iterator first = s.begin(), last = s.end();
		yak::spirit::packrat_rule<std::string::iterator> pr;
		pr = qi::byte_ >> qi::word >> qi::dword >> qi::qword >> qi::byte_(0x11) >> qi::word(0x1312U) >> qi::dword(0x17161514UL) >> qi::qword(0x1f1e1d1c1b1a1918ULL);
		BOOST_CHECK( qi::parse(first, last, pr));

		s = "\x01";
		first = s.begin(), last = s.end();
		pr = &qi::byte_(1) >> qi::byte_(2);
		BOOST_CHECK(!qi::parse(first, last, pr));

		s = "\x01\x02";
		first = s.begin(), last = s.end();
		pr = &qi::word(0x0201U) >> qi::word(0x0102U);
		BOOST_CHECK(!qi::parse(first, last, pr));

		s = "\x01\x02\x03\x04";
		first = s.begin(), last = s.end();
		pr = &qi::dword(0x04030201UL) >> qi::dword(0x01020304UL);
		BOOST_CHECK(!qi::parse(first, last, pr));

		s = "\x01\x02\x03\x04\x05\x06\x07\x08";
		first = s.begin(), last = s.end();
		pr = &qi::qword(0x0807060504030201ULL) >> qi::qword(0x0102030405060708ULL);
		BOOST_CHECK(!qi::parse(first, last, pr));

		YAK_SPIRIT_PACKRAT_TEST_BLOCK
		{
			s = "\x01\x02\x03\x04";
			first = s.begin(), last = s.end();
			pr = &qi::dword(0x04030201U) >> qi::dword(0x04030201U);
			BOOST_CHECK( qi::parse(first, last, pr));
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
		}
	} else {
		std::string s = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
		std::string::iterator first = s.begin(), last = s.end();
		yak::spirit::packrat_rule<std::string::iterator> pr;
		pr = qi::byte_ >> qi::word >> qi::dword >> qi::qword >> qi::byte_(0x11) >> qi::word(0x1213U) >> qi::dword(0x14151617UL) >> qi::qword(0x18191a1b1c1d1e1fULL);
		BOOST_CHECK( qi::parse(first, last, pr));

		s = "\x01";
		first = s.begin(), last = s.end();
		pr = &qi::byte_(1) >> qi::byte_(2);
		BOOST_CHECK(!qi::parse(first, last, pr));

		s = "\x01\x02";
		first = s.begin(), last = s.end();
		pr = &qi::word(0x0102U) >> qi::word(0x0201U);
		BOOST_CHECK(!qi::parse(first, last, pr));

		s = "\x01\x02\x03\x04";
		first = s.begin(), last = s.end();
		pr = &qi::dword(0x01020304UL) >> qi::dword(0x04030201UL);
		BOOST_CHECK(!qi::parse(first, last, pr));

		s = "\x01\x02\x03\x04\x05\x06\x07\x08";
		first = s.begin(), last = s.end();
		pr = &qi::qword(0x0102030405060708ULL) >> qi::qword(0x0807060504030201ULL);
		BOOST_CHECK(!qi::parse(first, last, pr));

		YAK_SPIRIT_PACKRAT_TEST_BLOCK
		{
			s = "\x01\x02\x03\x04";
			first = s.begin(), last = s.end();
			pr = &qi::dword(0x01020304U) >> qi::dword(0x01020304U);
			BOOST_CHECK( qi::parse(first, last, pr));
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
			BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
		}
	}
}

BOOST_AUTO_TEST_CASE(test_lebin_parsers)
{
	namespace qi = boost::spirit::qi;

	std::string s = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
	std::string::iterator first = s.begin(), last = s.end();
	yak::spirit::packrat_rule<std::string::iterator> pr;
	pr = qi::byte_ >> qi::little_word >> qi::little_dword >> qi::little_qword >> qi::byte_(0x11) >> qi::little_word(0x1312U) >> qi::little_dword(0x17161514UL) >> qi::little_qword(0x1f1e1d1c1b1a1918ULL);
	BOOST_CHECK( qi::parse(first, last, pr));

	s = "\x01";
	first = s.begin(), last = s.end();
	pr = &qi::byte_(1) >> qi::byte_(2);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "\x01\x02";
	first = s.begin(), last = s.end();
	pr = &qi::little_word(0x0201U) >> qi::little_word(0x0102U);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "\x01\x02\x03\x04";
	first = s.begin(), last = s.end();
	pr = &qi::little_dword(0x04030201UL) >> qi::little_dword(0x01020304UL);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "\x01\x02\x03\x04\x05\x06\x07\x08";
	first = s.begin(), last = s.end();
	pr = &qi::little_qword(0x0807060504030201ULL) >> qi::little_qword(0x0102030405060708ULL);
	BOOST_CHECK(!qi::parse(first, last, pr));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "\x01\x02\x03\x04";
		first = s.begin(), last = s.end();
		pr = &qi::little_dword(0x04030201U) >> qi::little_dword(0x04030201U);
		BOOST_CHECK( qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}

BOOST_AUTO_TEST_CASE(test_bebin_parsers)
{
	namespace qi = boost::spirit::qi;

	std::string s = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
	std::string::iterator first = s.begin(), last = s.end();
	yak::spirit::packrat_rule<std::string::iterator> pr;
	pr = qi::byte_ >> qi::big_word >> qi::big_dword >> qi::big_qword >> qi::byte_(0x11) >> qi::big_word(0x1213U) >> qi::big_dword(0x14151617UL) >> qi::big_qword(0x18191a1b1c1d1e1fULL);
	BOOST_CHECK( qi::parse(first, last, pr));

	s = "\x01";
	first = s.begin(), last = s.end();
	pr = &qi::byte_(1) >> qi::byte_(2);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "\x01\x02";
	first = s.begin(), last = s.end();
	pr = &qi::big_word(0x0102U) >> qi::big_word(0x0201U);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "\x01\x02\x03\x04";
	first = s.begin(), last = s.end();
	pr = &qi::big_dword(0x01020304UL) >> qi::big_dword(0x04030201UL);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "\x01\x02\x03\x04\x05\x06\x07\x08";
	first = s.begin(), last = s.end();
	pr = &qi::big_qword(0x0102030405060708ULL) >> qi::big_qword(0x0807060504030201ULL);
	BOOST_CHECK(!qi::parse(first, last, pr));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "\x01\x02\x03\x04";
		first = s.begin(), last = s.end();
		pr = &qi::big_dword(0x01020304U) >> qi::big_dword(0x01020304U);
		BOOST_CHECK( qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}
