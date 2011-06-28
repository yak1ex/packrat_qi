/***********************************************************************/
/*                                                                     */
/* packrat_rule_debug.hpp: Packrat parser on Spirit - debug support    */
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

#ifndef YAK_SPIRIT_PACKRAT_RULE_DEBUG_HPP
#define YAK_SPIRIT_PACKRAT_RULE_DEBUG_HPP

#include "packrat_rule.hpp"

#include <boost/spirit/home/qi/nonterminal/debug_handler.hpp>
#include <boost/spirit/home/qi/nonterminal/error_handler.hpp>
#include <boost/spirit/home/qi/nonterminal/simple_trace.hpp>

namespace boost { namespace spirit { namespace qi {

	template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename F>
	void debug(yak::spirit::packrat_rule<Iterator, T1, T2, T3, T4>& r, F f)
	{
		typedef yak::spirit::packrat_rule<Iterator, T1, T2, T3, T4> rule_type;

		typedef
			debug_handler<
				Iterator
			  , typename rule_type::context_type
			  , typename rule_type::skipper_type
			  , F>
		debug_handler;
		r.f = debug_handler(r.f, f, r.name());
	}

	template <typename Iterator, typename T1, typename T2, typename T3, typename T4>
	void debug(yak::spirit::packrat_rule<Iterator, T1, T2, T3, T4>& r)
	{
		typedef yak::spirit::packrat_rule<Iterator, T1, T2, T3, T4> rule_type;

		typedef
			debug_handler<
				Iterator
			  , typename rule_type::context_type
			  , typename rule_type::skipper_type
			  , simple_trace>
		debug_handler;

		typedef typename qi::detail::get_simple_trace<Iterator>::type trace;
		r.f = debug_handler(r.f, trace(), r.name());
	}

	template <
		error_handler_result action,
		typename Iterator, typename T0, typename T1, typename T2,
		typename F>
	void on_error(yak::spirit::packrat_rule<Iterator, T0, T1, T2>& r, F f)
	{
		typedef yak::spirit::packrat_rule<Iterator, T0, T1, T2> rule_type;

		typedef
			error_handler<
				Iterator,
				typename rule_type::context_type,
				typename rule_type::skipper_type,
				F,
				action>
		error_handler;
		r.f = error_handler(r.f, f);
	}

	// Error handling support when <action> is not
	// specified. We will default to <fail>.
	template <typename Iterator, typename T0, typename T1
	  , typename T2, typename F>
	void on_error(yak::spirit::packrat_rule<Iterator, T0, T1, T2>& r, F f)
	{
		on_error<fail>(r, f);
	}

}}}

#endif
