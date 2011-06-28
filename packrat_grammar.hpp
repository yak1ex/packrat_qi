/***********************************************************************/
/*                                                                     */
/* packrat_grammar.hpp: Packrat parser on Spirit - grammar part        */
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

#ifndef YAK_SPIRIT_PACKRAT_GRAMMAR_HPP
#define YAK_SPIRIT_PACKRAT_GRAMMAR_HPP

#include <boost/spirit/include/qi_grammar.hpp>
#include "packrat_core.hpp"

namespace yak { namespace spirit {

	template <
		typename Iterator,
		typename T1 = boost::spirit::unused_type,
		typename T2 = boost::spirit::unused_type,
		typename T3 = boost::spirit::unused_type,
		typename T4 = boost::spirit::unused_type>
	struct packrat_grammar
	  : boost::proto::extends<
			typename boost::proto::terminal<
				boost::spirit::qi::reference<packrat_rule<Iterator, T1, T2, T3, T4> const>
			>::type
		  , packrat_grammar<Iterator, T1, T2, T3, T4>
		>
	  , boost::spirit::qi::parser<packrat_grammar<Iterator, T1, T2, T3, T4> >
	  , boost::noncopyable
	{
		typedef Iterator iterator_type;
		typedef packrat_rule<Iterator, T1, T2, T3, T4> start_type;
		typedef typename start_type::sig_type sig_type;
		typedef typename start_type::locals_type locals_type;
		typedef typename start_type::skipper_type skipper_type;
		typedef typename start_type::encoding_type encoding_type;
		typedef packrat_grammar<Iterator, T1, T2, T3, T4> base_type;
		typedef boost::spirit::qi::reference<start_type const> reference_;
		typedef typename boost::proto::terminal<reference_>::type terminal;

		static std::size_t const params_size = start_type::params_size;

		template <typename Context, typename Iterator_>
		struct attribute
		{
			typedef typename start_type::attr_type type;
		};

		packrat_grammar(
			start_type const& start
		  , std::string const& name_ = "unnamed-grammar")
		: boost::proto::extends<terminal, base_type>(terminal::make(reference_(start)))
		, name_(name_)
		{}

		// This constructor is used to catch if the start rule is not
		// compatible with the grammar.
		template <typename Iterator_,
			typename T1_, typename T2_, typename T3_, typename T4_>
		packrat_grammar(
			packrat_rule<Iterator_, T1_, T2_, T3_, T4_> const&
		  , std::string const& = "unnamed-grammar")
		{
			// If you see the assertion below failing then the start rule
			// passed to the constructor of the grammar is not compatible with
			// the grammar (i.e. it uses different template parameters).
			BOOST_SPIRIT_ASSERT_MSG(
				(boost::is_same<start_type, packrat_rule<Iterator_, T1_, T2_, T3_, T4_> >::value)
			  , incompatible_start_rule, (packrat_rule<Iterator_, T1_, T2_, T3_, T4_>));
		}

		std::string name() const
		{
			return name_;
		}

		void name(std::string const& str)
		{
			name_ = str;
		}

		template <typename Context, typename Skipper, typename Attribute>
		bool parse(Iterator& first, Iterator const& last
		  , Context& context, Skipper const& skipper
		  , Attribute& attr) const
		{
			return this->proto_base().child0.parse(
				first, last, context, skipper, attr);
		}

		template <typename Context>
		boost::spirit::info what(Context& context) const
		{
			return boost::spirit::info(name_);
		}

		// bring in the operator() overloads
		start_type const& get_parameterized_subject() const
		{ return this->proto_base().child0.ref.get(); }
		typedef start_type parameterized_subject_type;
		#include "detail/packrat_fcall.hpp"

		std::string name_;

	};
}}

namespace boost { namespace spirit { namespace traits
{
	///////////////////////////////////////////////////////////////////////////
	template <
		typename IteratorA, typename IteratorB, typename Attribute
	  , typename Context, typename T1, typename T2, typename T3, typename T4>
	struct handles_container<
		yak::spirit::packrat_grammar<IteratorA, T1, T2, T3, T4>, Attribute, Context, IteratorB>
	  : traits::is_container<
		  typename attribute_of<
			  yak::spirit::packrat_grammar<IteratorA, T1, T2, T3, T4>, Context, IteratorB
		  >::type
		>
	{};

}}}

#endif
