/***********************************************************************/
/*                                                                     */
/* test_packrat_numeric.cpp: Test code for Packrat parser - numeric p. */
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
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>

#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_real_parsers)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator, qi::space_type> pr1;
	pr1 = qi::float_ >> qi::double_ >> qi::long_double >> qi::lit(4.0) >> qi::float_(5.0) >> qi::double_(6.0) >> qi::long_double(7.0);
	std::string s("1.0 2.0 3.0 4.0 5.0 6.0 7.0");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::phrase_parse(first, last, pr1, qi::space));

	yak::spirit::packrat_rule<std::string::iterator> pr2;
	s = "1.0";
	first = s.begin(), last = s.end();
	pr2 = &qi::lit(1.0) >> qi::lit(2.0);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1.0";
	first = s.begin(), last = s.end();
	pr2 = &qi::float_(1.0) >> qi::float_(2.0);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1.0";
	first = s.begin(), last = s.end();
	pr2 = &qi::double_(1.0) >> qi::double_(2.0);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1.0";
	first = s.begin(), last = s.end();
	pr2 = &qi::long_double(1.0) >> qi::long_double(2.0);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "1.0";
		first = s.begin(), last = s.end();
		pr2 = &qi::double_ >> qi::double_;
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

}

BOOST_AUTO_TEST_CASE(test_int_parsers)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator, qi::space_type> pr1;
	pr1 = qi::short_ >> qi::int_ >> qi::long_ >> qi::long_long >> qi::lit(2147483647L) >> qi::short_(32767) >> qi::int_(32767) >> qi::long_(2147483647L) >> qi::long_long(2147483647L);
	std::string s("32767 32767 2147483647 2147483647 2147483647 32767 32767 2147483647 2147483647");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::phrase_parse(first, last, pr1, qi::space));

	yak::spirit::packrat_rule<std::string::iterator> pr2;
	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::lit(1) >> qi::lit(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::short_(1) >> qi::short_(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::int_(1) >> qi::int_(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::long_(1) >> qi::long_(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::long_long(1) >> qi::long_long(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "1";
		first = s.begin(), last = s.end();
		pr2 = &qi::int_ >> qi::int_;
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}

BOOST_AUTO_TEST_CASE(test_uint_parsers)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator, qi::space_type> pr1;
	pr1 = qi::ushort_ >> qi::uint_ >> qi::ulong_ >> qi::ulong_long >> qi::lit(4294967295UL) >> qi::ushort_(65535U) >> qi::uint_(65535U) >> qi::ulong_(4294967295UL) >> qi::ulong_long(4294967295UL);
	std::string s("65535 65535 4294967295 4294967295 4294967295 65535 65535 4294967295 4294967295");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::phrase_parse(first, last, pr1, qi::space));

	pr1 = qi::bin >> qi::oct >> qi::hex >> qi::bin(65535) >> qi::oct(65535) >> qi::hex(65535);
	s = ("1010 0777 FFFF 1111111111111111 177777 FFFF");
	first = s.begin(); last = s.end();
	BOOST_CHECK(qi::phrase_parse(first, last, pr1, qi::space));

	yak::spirit::packrat_rule<std::string::iterator> pr2;
	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::lit(1U) >> qi::lit(2U);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::ushort_(1) >> qi::ushort_(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::uint_(1) >> qi::uint_(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::ulong_(1) >> qi::ulong_(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::ulong_long(1) >> qi::ulong_long(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::bin(1) >> qi::bin(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::oct(1) >> qi::oct(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "1";
	first = s.begin(), last = s.end();
	pr2 = &qi::hex(1) >> qi::hex(2);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "1";
		first = s.begin(), last = s.end();
		pr2 = &qi::uint_(1) >> qi::uint_(1);
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "100";
		first = s.begin(), last = s.end();
		pr2 = &qi::hex(256) >> qi::hex(256);
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}

BOOST_AUTO_TEST_CASE(test_bool_parsers)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator, qi::space_type> pr1;
	pr1 = qi::bool_ >> qi::lit(true) >> qi::bool_(false) >> qi::true_ >> qi::false_;
	std::string s("true true false true false");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::phrase_parse(first, last, pr1, qi::space));

	yak::spirit::packrat_rule<std::string::iterator> pr2;
	s = "true";
	first = s.begin(), last = s.end();
	pr2 = &qi::lit(true) >> qi::lit(false);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "true";
	first = s.begin(), last = s.end();
	pr2 = &qi::bool_(true) >> qi::bool_(false);
	BOOST_CHECK(!qi::parse(first, last, pr2));

	s = "true";
	first = s.begin(), last = s.end();
	pr2 = &qi::true_ >> qi::false_;
	BOOST_CHECK(!qi::parse(first, last, pr2));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "true";
		first = s.begin(), last = s.end();
		pr2 = &qi::bool_(true) >> qi::bool_(true);
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "true";
		first = s.begin(), last = s.end();
		pr2 = &qi::true_ >> qi::true_;
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}
