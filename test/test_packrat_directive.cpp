/***********************************************************************/
/*                                                                     */
/* test_packrat_directive.cpp: Test code for Packrat parser - directive*/
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
#include <boost/spirit/include/qi_directive.hpp>

#include <boost/test/auto_unit_test.hpp>

// Do not affect skipper in arguments
// Changing skipper during parsing may cause inproper result

BOOST_AUTO_TEST_CASE(test_packrat_directive)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator, qi::space_type> pr1;
	yak::spirit::packrat_rule<std::string::iterator> pr2;
	yak::spirit::packrat_rule<std::string::iterator, qi::space_type> pr3;
	pr3 = qi::raw[qi::repeat(2, qi::inf)[qi::char_('c')] >> qi::as_string[qi::repeat(2, qi::inf)[qi::char_('d')]]];
	pr2 = qi::matches[qi::char_('a')] >> qi::hold[qi::repeat(3, 5)[qi::char_('b')]] >> qi::skip(qi::space)[pr3];
	pr1 = qi::lexeme[qi::repeat[qi::alnum]] >> qi::no_case[qi::omit[qi::repeat(3)[qi::char_('a')]]] >> qi::no_skip[pr2 >> qi::skip[pr3]];
	std::string s("abc123 AAAabbb cc dd ccc ddd");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::phrase_parse(first, last, pr1, qi::space));

	pr2 = &qi::repeat(3)[qi::char_('a')] >> qi::repeat(4)[qi::char_('a')];
	s = "aaa";
	first = s.begin(); last = s.end();
	BOOST_CHECK(!qi::parse(first, last, pr2));

	pr2 = &qi::repeat(3,4)[qi::char_('a')] >> qi::repeat(5,6)[qi::char_('a')];
	s = "aaaa";
	first = s.begin(); last = s.end();
	BOOST_CHECK(!qi::parse(first, last, pr2));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		pr2 = &qi::skip(qi::digit)[qi::repeat(2)[qi::char_('a')]] >> qi::skip(qi::alpha)[qi::char_('1')];
		s = "a1a";
		first = s.begin(); last = s.end();
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 8);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "aaa";
		first = s.begin(), last = s.end();
		pr2 = &qi::repeat(3)[qi::char_] >> qi::repeat(3)[qi::char_];
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 6);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "aaaaa";
		first = s.begin(), last = s.end();
		pr2 = &qi::repeat(3)[qi::char_] >> qi::repeat(3, 5)[qi::char_];
		BOOST_CHECK( qi::parse(first, last, pr2));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 9);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}
