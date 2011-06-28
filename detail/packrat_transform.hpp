/***********************************************************************/
/*                                                                     */
/* packrat_transform.hpp: Packrat parser on Spirit - tree transform    */
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

#ifndef YAK_SPIRIT_DETAIL_PACKRAT_TRANSFORM_HPP
#define YAK_SPIRIT_DETAIL_PACKRAT_TRANSFORM_HPP

#include <boost/fusion/include/at_c.hpp> // need for make_expr

#include <boost/proto/matches.hpp>
#include <boost/proto/transform/when.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/transform/arg.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/proto/traits.hpp>

#include <boost/spirit/include/qi_domain.hpp>
#include <boost/spirit/home/support/terminal.hpp>

namespace yak { namespace spirit { namespace detail {

	// See also, boost/spirit/home/support/meta_compiler.hpp

	struct packratifier
	{
		struct meta_grammar;

		struct cases
		{
			template<typename Tag, typename Enable = void>
			struct case_ : boost::proto::not_<boost::proto::_> {};

			template<typename Enable>
			struct case_<boost::proto::tag::terminal, Enable> :
				boost::proto::when<
					boost::proto::if_<
						boost::spirit::use_terminal<
							boost::spirit::qi::domain,
							boost::proto::_value
						>()
					>,
					boost::proto::_make_subscript(
						boost::proto::_state,
						boost::proto::_expr
					)
				> {};

			template<typename Tag>
			struct case_<Tag, typename boost::enable_if<
					boost::spirit::use_operator<
						boost::spirit::qi::domain, Tag
					>
				>::type
			> : boost::proto::or_<
					boost::proto::when<
						boost::proto::binary_expr<Tag, meta_grammar, meta_grammar>,
						boost::proto::_make_subscript(
							boost::proto::_state,
							boost::proto::binary_expr<Tag, meta_grammar, meta_grammar>
						)
					>,
					boost::proto::when<
						boost::proto::unary_expr<Tag, meta_grammar>,
						boost::proto::_make_subscript(
							boost::proto::_state,
							boost::proto::unary_expr<Tag, meta_grammar>
						)
					>
				> {};

			template<typename Enable>
			struct case_<boost::proto::tag::subscript, Enable> :
				boost::proto::or_<
					boost::proto::when<
						boost::proto::binary_expr<
							boost::proto::tag::subscript,
							boost::proto::and_<
								boost::proto::terminal<boost::proto::_>,
								boost::proto::if_<
									boost::spirit::use_directive<
										boost::spirit::qi::domain,
										boost::proto::_value
									>()
								>
							>,
							meta_grammar
						>,
						boost::proto::_make_subscript(
							boost::proto::_state,
							boost::proto::_make_subscript(
								boost::proto::_child_c<0>,
								meta_grammar(boost::proto::_child_c<1>)
							)
						)
					>,
					boost::proto::when<
						boost::proto::binary_expr<
							boost::proto::tag::subscript,
							meta_grammar,
							boost::proto::_
						>,
						boost::proto::_make_subscript(
							boost::proto::_state,
							boost::proto::_make_subscript(
								meta_grammar(boost::proto::_child_c<0>),
								boost::proto::_child_c<1>
							)
						)
					>
				> {};
		};
		struct meta_grammar : boost::proto::switch_<cases> {};
	};

}}}

#endif
