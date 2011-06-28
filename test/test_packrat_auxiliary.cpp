/***********************************************************************/
/*                                                                     */
/* test_packrat_auxiliary.cpp: Test code for Packrat parser - aux p.   */
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
#include <boost/spirit/include/qi_auxiliary.hpp>

#include <boost/spirit/home/phoenix/core/value.hpp>

#include <boost/test/auto_unit_test.hpp>

// Do not support lazy parsers
// Do not support recursive application on subject of attr_cast

BOOST_AUTO_TEST_CASE(test_auxiliary_parsers)
{
	namespace qi = boost::spirit::qi;

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		yak::spirit::packrat_rule<std::string::iterator> pr;
		pr = qi::attr_cast(qi::char_) >> qi::eol >> qi::eps >> qi::eoi >> qi::eps(boost::phoenix::val(true)) >> qi::attr(1);
		std::string s("\n\n");
		std::string::iterator first = s.begin(), last = s.end();
		BOOST_CHECK(qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 11);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	std::string s = "";
	std::string::iterator first = s.begin(), last = s.end();
	yak::spirit::packrat_rule<std::string::iterator> pr = &qi::eps(true) >> qi::eps(false);
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "";
	first = s.begin(); last = s.end();
	yak::spirit::packrat_rule<std::string::iterator, int()> pr2 = &qi::attr(1) >> qi::attr(2);
	int result;
	BOOST_CHECK( qi::parse(first, last, pr2, result));
	BOOST_CHECK_EQUAL(result, 2);
}
