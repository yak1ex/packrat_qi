/***********************************************************************/
/*                                                                     */
/* test_packrat.cpp: Test code for packrat parser on Spirit            */
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
#include "packrat_grammar.hpp"
#include "packrat_rule_debug.hpp"

#include <string>

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_action.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/progress.hpp>

#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_packrat_parser_construct)
{
	using namespace boost::spirit::qi;

	yak::spirit::packrat_parser<int_parser<> >
		pp((int_parser<>()));
}

BOOST_AUTO_TEST_CASE(test_packrat_rule)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator> pr1;
	pr1 = &qi::int_ >> &qi::int_ >> qi::int_;

	std::string s("123");
	std::string::iterator first = s.begin(), last = s.end();

	BOOST_CHECK(qi::parse(first, last, pr1));


	yak::spirit::packrat_rule<std::string::iterator, int()> pr2;
	pr2 = &qi::int_ >> &qi::int_ >> qi::int_;
	int result = 0;
	first = s.begin(); last = s.end();

	BOOST_CHECK(qi::parse(first, last, pr2, result));
	BOOST_CHECK_EQUAL(result, 123);

	yak::spirit::packrat_rule<std::string::iterator, int(), qi::space_type> pr3;
	pr3 = pr1 >> pr2;
	s = "456 123";
	first = s.begin(); last = s.end();

	BOOST_CHECK(qi::phrase_parse(first, last, pr3, qi::space, result));
	BOOST_CHECK_EQUAL(result, 123);
}

BOOST_AUTO_TEST_CASE(test_packrat_rule_debug)
{
	namespace qi = boost::spirit::qi;

	yak::spirit::packrat_rule<std::string::iterator> pr1;
	pr1 = qi::int_;
	yak::spirit::packrat_rule<std::string::iterator, int()> pr2;
	pr2 = qi::int_;
	yak::spirit::packrat_rule<std::string::iterator, int(), qi::space_type> pr3;
	pr3 = pr1 >> pr2;
	boost::spirit::qi::debug(pr1);
	boost::spirit::qi::debug(pr2);
	boost::spirit::qi::debug(pr3);

	std::string s("123 456");
	std::string::iterator first = s.begin(), last = s.end();
	int result = 0;
	std::string debug_result(
"<unnamed-rule>\n"
"  <try>123 456</try>\n"
"  <unnamed-rule>\n"
"    <try>123 456</try>\n"
"    <success> 456</success>\n"
"    <attributes>[]</attributes>\n"
"  </unnamed-rule>\n"
"  <unnamed-rule>\n"
"    <try>456</try>\n"
"    <success></success>\n"
"    <attributes>[456]</attributes>\n"
"  </unnamed-rule>\n"
"  <success></success>\n"
"  <attributes>[456]</attributes>\n"
"</unnamed-rule>\n"
);

	BOOST_CHECK(qi::phrase_parse(first, last, pr3, qi::space, result));
	BOOST_CHECK_EQUAL(result, 456);
	BOOST_CHECK_EQUAL(yak::spirit::debug::test_ss.str(), debug_result);
}

template<typename Iterator>
struct expr_grammar : yak::spirit::packrat_grammar<Iterator, int(), boost::spirit::qi::locals<int, char> >
{
	expr_grammar() : expr_grammar::base_type(expr)
	{
		namespace qi = boost::spirit::qi;
		namespace phoenix = boost::phoenix;
		using namespace qi::labels;

		expr = term[_a = _1] >> qi::char_("+-")[_b = _1] >> expr[_val = phoenix::if_else(_b == '+', _a + _1, _a - _1)]  | term[_val = _1];
		term = factor[_a = _1] >> qi::char_("*/")[_b = _1] >> term[_val = phoenix::if_else(_b == '*', _a * _1, _a / _1)] | factor[_val = _1];
		factor = qi::int_ | qi::lit('(') >> expr >> qi::lit(')');
	}
	yak::spirit::packrat_rule<Iterator, int(), boost::spirit::qi::locals<int, char> > expr;
	yak::spirit::packrat_rule<Iterator, int(), boost::spirit::qi::locals<int, char> > term;
	yak::spirit::packrat_rule<Iterator, int()> factor;
};

template<typename Iterator>
struct expr_grammar_ : boost::spirit::qi::grammar<Iterator, int(), boost::spirit::qi::locals<int, char> >
{
	expr_grammar_() : expr_grammar_::base_type(expr)
	{
		namespace qi = boost::spirit::qi;
		namespace phoenix = boost::phoenix;
		using namespace qi::labels;

		expr = term[_a = _1] >> qi::char_("+-")[_b = _1] >> expr[_val = phoenix::if_else(_b == '+', _a + _1, _a - _1)]  | term[_val = _1];
		term = factor[_a = _1] >> qi::char_("*/")[_b = _1] >> term[_val = phoenix::if_else(_b == '*', _a * _1, _a / _1)] | factor[_val = _1];
		factor = qi::int_ | qi::lit('(') >> expr >> qi::lit(')');
	}
	boost::spirit::qi::rule<Iterator, int(), boost::spirit::qi::locals<int, char> > expr;
	boost::spirit::qi::rule<Iterator, int(), boost::spirit::qi::locals<int, char> > term;
	boost::spirit::qi::rule<Iterator, int()> factor;
};

BOOST_AUTO_TEST_CASE(test_packrat_grammar)
{
	namespace qi = boost::spirit::qi;

	expr_grammar<std::string::iterator> g;
	std::string s("1*2+3-4*5-(4/2-1)");
	std::string::iterator first = s.begin(), last = s.end();
	int result = 0;

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		std::cout << "Packrat: Expression with " << s.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g, result));
		}
		BOOST_CHECK_EQUAL(result, -14);
		yak::spirit::debug::count::out(std::cout);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		std::string s2 = s + "+" + s;
		first = s2.begin(); last = s2.end();
		std::cout << "Packrat: Expression with " << s2.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g, result));
		}
		BOOST_CHECK_EQUAL(result, -28);
		yak::spirit::debug::count::out(std::cout);
	}

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		std::string s2 = s, s3 = "+" + s;
		for(int i=0;i<255;++i) {
			s2 += s3;
		}
		first = s2.begin(); last = s2.end();
		std::cout << "Packrat: Expression with " << s2.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g, result));
		}
		BOOST_CHECK_EQUAL(result, -3584);
		yak::spirit::debug::count::out(std::cout);
	}

	{
		expr_grammar_<std::string::iterator> g_;
		std::string s2 = s, s3 = "+" + s;
		for(int i=0;i<255;++i) {
			s2 += s3;
		}
		first = s2.begin(); last = s2.end();
		std::cout << "Plain: Expression with " << s2.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g_, result));
		}
		BOOST_CHECK_EQUAL(result, -3584);
	}
}

template<typename Iterator>
struct test_grammar : yak::spirit::packrat_grammar<Iterator>
{
	test_grammar() : test_grammar::base_type(e)
	{
		namespace qi = boost::spirit::qi;

		a = b | c;
		b = qi::char_('a') >> b | qi::char_('c');
		c = qi::char_('a') >> c | qi::char_('d');
		d = &a >> qi::char_('a');
		e = qi::char_('d') | d >> e;
	}
	yak::spirit::packrat_rule<Iterator> a;
	yak::spirit::packrat_rule<Iterator> b;
	yak::spirit::packrat_rule<Iterator> c;
	yak::spirit::packrat_rule<Iterator> d;
	yak::spirit::packrat_rule<Iterator> e;
};

template<typename Iterator>
struct test_grammar_ : boost::spirit::qi::grammar<Iterator>
{
	test_grammar_() : test_grammar_::base_type(e)
	{
		namespace qi = boost::spirit::qi;

		a = b | c;
		b = qi::char_('a') >> b | qi::char_('c');
		c = qi::char_('a') >> c | qi::char_('d');
		d = &a >> qi::char_('a');
		e = qi::char_('d') | d >> e;
	}
	boost::spirit::qi::rule<Iterator> a;
	boost::spirit::qi::rule<Iterator> b;
	boost::spirit::qi::rule<Iterator> c;
	boost::spirit::qi::rule<Iterator> d;
	boost::spirit::qi::rule<Iterator> e;
};

BOOST_AUTO_TEST_CASE(test_packrat_complexity)
{
	namespace qi = boost::spirit::qi;

	std::string s(512, 'a');
	s += "d";
	std::string::iterator first = s.begin(), last = s.end();
	test_grammar<std::string::iterator> g;
	test_grammar_<std::string::iterator> g_;

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		std::cout << "Packrat: Quadratic with " << s.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g));
		}
		yak::spirit::debug::count::out(std::cout);
	}

	{
		first = s.begin(); last = s.end();
		std::cout << "Plain: Quadratic with " << s.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g_));
		}
	}

	std::string s2(1024, 'a');
	s = s2 + "d";

	YAK_SPIRIT_PACKRAT_TEST_BLOCK
	{
		first = s.begin(); last = s.end();
		std::cout << "Packrat: Quadratic with " << s.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g));
		}
		yak::spirit::debug::count::out(std::cout);
	}

	{
		first = s.begin(); last = s.end();
		std::cout << "Plain: Quadratic with " << s.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g_));
		}
	}
}

template<typename Iterator>
struct test_grammar2 : yak::spirit::packrat_grammar<Iterator>
{
	test_grammar2() : test_grammar2::base_type(e)
	{
		namespace qi = boost::spirit::qi;

		a = b | c;
		b = qi::char_('a') >> &a >> b | qi::char_('c');
		c = qi::char_('a') >> &a >> c | qi::char_('d');
		d = &a >> qi::char_('a');
		e = qi::char_('d') | d >> e;
	}
	yak::spirit::packrat_rule<Iterator> a;
	yak::spirit::packrat_rule<Iterator> b;
	yak::spirit::packrat_rule<Iterator> c;
	yak::spirit::packrat_rule<Iterator> d;
	yak::spirit::packrat_rule<Iterator> e;
};

template<typename Iterator>
struct test_grammar2_ : boost::spirit::qi::grammar<Iterator>
{
	test_grammar2_() : test_grammar2_::base_type(e)
	{
		namespace qi = boost::spirit::qi;

		a = b | c;
		b = qi::char_('a') >> &a >> b | qi::char_('c');
		c = qi::char_('a') >> &a >> c | qi::char_('d');
		d = &a >> qi::char_('a');
		e = qi::char_('d') | d >> e;
	}
	boost::spirit::qi::rule<Iterator> a;
	boost::spirit::qi::rule<Iterator> b;
	boost::spirit::qi::rule<Iterator> c;
	boost::spirit::qi::rule<Iterator> d;
	boost::spirit::qi::rule<Iterator> e;
};

BOOST_AUTO_TEST_CASE(test_packrat_complexity2)
{
	namespace qi = boost::spirit::qi;

	std::string s;
	std::string::iterator first, last;
	test_grammar2<std::string::iterator> g;
	test_grammar2_<std::string::iterator> g_;

	for(int i=8;i<=12;++i) {
		std::string s2(i, 'a');
		s = s2 + "d";

		first = s.begin(); last = s.end();
		std::cout << "Packrat: Exponential with " << s.size() << " chars." << std::endl;
		YAK_SPIRIT_PACKRAT_TEST_BLOCK
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g));
		}
		yak::spirit::debug::count::out(std::cout);

		first = s.begin(); last = s.end();
		std::cout << "Plain: Exponential with " << s.size() << " chars." << std::endl;
		{
			boost::progress_timer t;
			BOOST_CHECK(qi::parse(first, last, g_));
		}
	}
}
