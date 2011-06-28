/***********************************************************************/
/*                                                                     */
/* test_packrat_operator.cpp: Test code for Packrat parser - operator  */
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

BOOST_AUTO_TEST_CASE(test_packrat_operator)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator> pr;
	std::string s;
	std::string::iterator first, last;

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "bbbbbb";
		first = s.begin(); last = s.end();
		pr = !(qi::char_('a') > *qi::char_('b') >> (qi::char_ - qi::char_('c'))) >>
				&((qi::char_('a') || qi::char_('b')) >> -qi::char_('c')) >>
				qi::char_('a') % qi::char_('b') |
				(qi::char_('a') ^ +qi::char_('b'));
		BOOST_CHECK(qi::parse(first, last, pr));
		// Not examined yet
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 15);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 7);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 3);
	}

	pr = &(qi::char_('a') | qi::char_('b')) >> (qi::char_('a') | qi::char_('c'));
	s = "b";
	first = s.begin(); last = s.end();
	BOOST_CHECK(!qi::parse(first, last, pr));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		pr = qi::char_('a') >> qi::char_('b') | qi::char_('a') >> qi::char_('b') | qi::char_('a') >> qi::char_('c');
		s = "ac";
		first = s.begin(); last = s.end();
		BOOST_CHECK( qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 4);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 1);
	}
}
