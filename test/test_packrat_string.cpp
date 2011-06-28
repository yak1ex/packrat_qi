/***********************************************************************/
/*                                                                     */
/* test_packrat_string.cpp: Test code for Packrat parser - str parser  */
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
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_action.hpp>

#include <boost/spirit/home/phoenix/bind/bind_function.hpp>

#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_string_parsers)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator> pr;
	pr = qi::lit("abcdefg") >> std::string("hijklmn") >> qi::string("opqrstu");
	std::string s("abcdefghijklmnopqrstu");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(qi::parse(first, last, pr));

	s = "abc";
	first = s.begin(), last = s.end();
	pr = &qi::lit("abc") >> qi::lit("def");
	BOOST_CHECK(!qi::parse(first, last, pr));

	s = "abc";
	first = s.begin(), last = s.end();
	pr = &qi::string("abc") >> qi::string("def");
	BOOST_CHECK(!qi::parse(first, last, pr));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "abc";
		first = s.begin(), last = s.end();
		yak::spirit::packrat_rule<std::string::iterator, std::string()> pr2;
		pr2 = &qi::string("abc") >> qi::string("abc");
		std::string result;
		BOOST_CHECK( qi::parse(first, last, pr2, result));
		BOOST_CHECK_EQUAL(result, std::string("abc"));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}

void adder(boost::spirit::qi::symbols<char, int>& sym, const char *s, int n)
{
	sym.add(s, n);
}

BOOST_AUTO_TEST_CASE(test_symbol_parsers)
{
	namespace qi = boost::spirit::qi;

	qi::symbols<char, int> sym1, sym2;
	sym1.add("a", 1)("b", 2);

// *** Do not support the case that symbols are changed during parsing ***
	yak::spirit::packrat_rule<std::string::iterator> pr;
	pr = !sym1 >> qi::eps[boost::phoenix::bind(&adder, boost::phoenix::ref(sym1), "c", 3)] >> sym1;
	std::string s("c");
	std::string::iterator first = s.begin(), last = s.end();
	BOOST_CHECK(!qi::parse(first, last, pr));

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
// *** Do not support comparision based on contents equality for symbols ***
		sym1.clear();
		sym1.add("a", 1)("b", 2);
		sym2 = sym1; // copy semantics
		pr = &sym1 >> sym2;
		s = "a";
		first = s.begin(); last = s.end();
		BOOST_CHECK( qi::parse(first, last, pr));
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 4);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		s = "a";
		first = s.begin(), last = s.end();
		yak::spirit::packrat_rule<std::string::iterator, int()> pr2;
		pr2 = &sym1 >> sym1;
		int result;
		BOOST_CHECK( qi::parse(first, last, pr2, result));
		BOOST_CHECK_EQUAL(result, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_success_, 3);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::parse_fail_, 0);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_success_, 1);
		BOOST_CHECK_EQUAL(yak::spirit::debug::count::hit_fail_, 0);
	}
}
