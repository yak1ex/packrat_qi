/***********************************************************************/
/*                                                                     */
/* test_packrat_char.cpp: Test code for Packrat parser - char parser   */
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

#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_character_parsers)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator> pr;
	pr = qi::lit('a') >> 'b' >> qi::char_ >> qi::char_('d') >> qi::char_("e") >> qi::char_('f', 'g') >> qi::char_("hij");
	std::string s("abcdegj");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::parse(first, last, pr));

	s = "0A \t1@";
	first = s.begin(), last = s.end();
	pr = qi::alnum >> qi::alpha >> qi::blank >> qi::cntrl >> qi::digit >> qi::graph;
	BOOST_CHECK(qi::parse(first, last, pr));

	s = " . FkL";
	first = s.begin(), last = s.end();
	pr = qi::print >> qi::punct >> qi::space >> qi::xdigit >> qi::lower >> qi::upper;
	BOOST_CHECK(qi::parse(first, last, pr));

	s = "o";
	first = s.begin(), last = s.end();
	pr = &qi::lit('o') >> qi::lit('x');
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "o";
	first = s.begin(), last = s.end();
	pr = &qi::char_('o') >> qi::char_('x');
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "o";
	first = s.begin(), last = s.end();
	pr = &qi::char_("o") >> qi::char_("x");
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "o";
	first = s.begin(), last = s.end();
	pr = &qi::char_('o', 'p') >> qi::char_('x', 'y');
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "o";
	first = s.begin(), last = s.end();
	pr = &qi::char_("op") >> qi::char_("xy");
	BOOST_CHECK(!qi::parse(first, last, pr));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "o";
		first = s.begin(), last = s.end();
		pr = &qi::char_ >> qi::char_;
		BOOST_CHECK( qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "o";
		first = s.begin(), last = s.end();
		pr = &qi::alnum >> qi::alnum;
		BOOST_CHECK( qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}
