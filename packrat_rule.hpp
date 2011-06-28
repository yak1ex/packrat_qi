/***********************************************************************/
/*                                                                     */
/* packrat_rule.hpp: Packrat parser on Spirit - rule part              */
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

#ifndef YAK_SPIRIT_PACKRAT_RULE_HPP
#define YAK_SPIRIT_PACKRAT_RULE_HPP

#include <boost/spirit/include/qi_rule.hpp>
#include "packrat_core.hpp"
#include "detail/packrat_transform.hpp"

namespace yak { namespace spirit {

	using boost::spirit::info;
	using boost::spirit::locals;

	template <
		typename Iterator,
		typename T1 = boost::spirit::unused_type,
		typename T2 = boost::spirit::unused_type,
		typename T3 = boost::spirit::unused_type,
		typename T4 = boost::spirit::unused_type>
	struct packrat_rule
	  : boost::proto::extends<
			typename boost::proto::terminal<
				boost::spirit::qi::reference<packrat_rule<Iterator, T1, T2, T3, T4> const>
			>::type
		  , packrat_rule<Iterator, T1, T2, T3, T4>
		>
	  , boost::spirit::qi::parser<packrat_rule<Iterator, T1, T2, T3, T4> >
	{
		typedef Iterator iterator_type;
		typedef packrat_rule<Iterator, T1, T2, T3, T4> this_type;
		typedef boost::spirit::qi::reference<this_type const> reference_;
		typedef typename boost::proto::terminal<reference_>::type terminal;
		typedef boost::proto::extends<terminal, this_type> base_type;
		typedef boost::mpl::vector<T1, T2, T3, T4> template_params;

		// The rule's locals_type: a sequence of types to be used as local variables
		typedef typename
			boost::spirit::detail::extract_locals<template_params>::type
		locals_type;

		// The rule's skip-parser type
		typedef typename
			boost::spirit::detail::extract_component<
				boost::spirit::qi::domain, template_params>::type
		skipper_type;

		// The rule's signature
		typedef typename
			boost::spirit::detail::extract_sig<template_params>::type
		sig_type;

		// The rule's encoding type
		typedef typename
			boost::spirit::detail::extract_encoding<template_params>::type
		encoding_type;

		// This is the rule's attribute type
		typedef typename
			boost::spirit::detail::attr_from_sig<sig_type>::type
		attr_type;
		typedef typename boost::add_reference<attr_type>::type attr_reference_type;

		// parameter_types is a sequence of types passed as parameters to the rule
		typedef typename
			boost::spirit::detail::params_from_sig<sig_type>::type
		parameter_types;

		static size_t const params_size =
			boost::fusion::result_of::size<parameter_types>::type::value;

		typedef context<
			Iterator,
			boost::fusion::cons<attr_reference_type, parameter_types>
		  , locals_type>
		context_type;

		typedef boost::function<
			bool(Iterator& first, Iterator const& last
			  , context_type& context
			  , skipper_type const& skipper
			)>
		function_type;

		typedef typename
			boost::mpl::if_<
				boost::is_same<encoding_type, boost::spirit::unused_type>
			  , boost::spirit::unused_type
			  , boost::spirit::tag::char_code<boost::spirit::tag::encoding, encoding_type>
			>::type
		encoding_modifier_type;

		explicit packrat_rule(std::string const& name_ = "unnamed-rule")
		  : base_type(terminal::make(reference_(*this)))
		  , name_(name_)
		{
		}

		packrat_rule(packrat_rule const& rhs)
		  : base_type(terminal::make(reference_(*this)))
		  , name_(rhs.name_)
		  , f(rhs.f)
		{
		}

		template <typename Expr>
		packrat_rule(Expr const& expr, std::string const& name_ = "unnamed-rule")
		  : base_type(terminal::make(reference_(*this)))
		  , name_(name_)
		{
			// Report invalid expression error as early as possible.
			// If you got an error_invalid_expression error message here,
			// then the expression (expr) is not a valid spirit qi expression.
			BOOST_SPIRIT_ASSERT_MATCH(boost::spirit::qi::domain, Expr);

			f = boost::spirit::qi::detail::bind_parser<boost::mpl::false_>(
				boost::spirit::compile<boost::spirit::qi::domain>(detail::packratifier::meta_grammar()(expr, packratify), encoding_modifier_type()));
		}

		packrat_rule& operator=(packrat_rule const& rhs)
		{
			// The following assertion fires when you try to initialize a rule
			// from an uninitialized one. Did you mean to refer to the right
			// hand side rule instead of assigning from it? In this case you
			// should write lhs = rhs.alias();
			BOOST_ASSERT(rhs.f && "Did you mean rhs.alias() instead of rhs?");

			f = rhs.f;
			name_ = rhs.name_;
			return *this;
		}

		std::string const& name() const
		{
			return name_;
		}

		void name(std::string const& str)
		{
			name_ = str;
		}

		template <typename Expr>
		packrat_rule& operator=(Expr const& expr)
		{
			// Report invalid expression error as early as possible.
			// If you got an error_invalid_expression error message here,
			// then the expression (expr) is not a valid spirit qi expression.
			BOOST_SPIRIT_ASSERT_MATCH(boost::spirit::qi::domain, Expr);

			f = boost::spirit::qi::detail::bind_parser<boost::mpl::false_>(
				boost::spirit::compile<boost::spirit::qi::domain>(detail::packratifier::meta_grammar()(expr, packratify), encoding_modifier_type()));
			return *this;
		}

// VC7.1 has problems to resolve 'rule' without explicit template parameters
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
		// g++ 3.3 barfs if this is a member function :(
		template <typename Expr>
		friend packrat_rule& operator%=(packrat_rule& r, Expr const& expr)
		{
			// Report invalid expression error as early as possible.
			// If you got an error_invalid_expression error message here,
			// then the expression (expr) is not a valid spirit qi expression.
			BOOST_SPIRIT_ASSERT_MATCH(boost::spirit::qi::domain, Expr);

			r.f = boost::spirit::qi::detail::bind_parser<boost::mpl::true_>(
				boost::spirit::compile<boost::spirit::qi::domain>(detail::packratifier::meta_grammar()(expr, packratify), encoding_modifier_type()));
			return r;
		}

		// non-const version needed to suppress proto's %= kicking in
		template <typename Expr>
		friend packrat_rule& operator%=(packrat_rule& r, Expr& expr)
		{
			return r %= static_cast<Expr const&>(expr);
		}
#else
		// both friend functions have to be defined out of class as VC7.1
		// will complain otherwise 
		template <typename OutputIterator_, typename T1_, typename T2_
		  , typename T3_, typename T4_, typename Expr>
		friend packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>& operator%=(
			packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>& r, Expr const& expr);

		// non-const version needed to suppress proto's %= kicking in
		template <typename OutputIterator_, typename T1_, typename T2_
		  , typename T3_, typename T4_, typename Expr>
		friend packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>& operator%=(
			packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>& r, Expr& expr);
#endif

		template <typename Context, typename Iterator_>
		struct attribute
		{
			typedef attr_type type;
		};

		template <typename Context, typename Skipper, typename Attribute>
		bool parse(Iterator& first, Iterator const& last
		  , Context& caller_context, Skipper const& skipper
		  , Attribute& attr) const
		{
			if (f)
			{
				// do a preskip if this is an implied lexeme
				if (boost::is_same<skipper_type, boost::spirit::unused_type>::value)
					boost::spirit::qi::skip_over(first, last, skipper);

				typedef boost::spirit::traits::make_attribute<attr_type, Attribute> make_attribute;

				// do down-stream transformation, provides attribute for
				// rhs parser
				typedef boost::spirit::traits::transform_attribute<
					typename make_attribute::type, attr_type, boost::spirit::qi::domain> 
				transform;

				typename make_attribute::type made_attr = make_attribute::call(attr);
				typename transform::type attr_ = transform::pre(made_attr);

				// If you are seeing a compilation error here, you are probably
				// trying to use a rule or a grammar which has inherited
				// attributes, without passing values for them.
				context_type context(attr_, caller_context);

				// If you are seeing a compilation error here stating that the
				// forth parameter can't be converted to a required target type
				// then you are probably trying to use a rule or a grammar with
				// an incompatible skipper type.
				if (f(first, last, context, skipper))
				{
					// do up-stream transformation, this integrates the results
					// back into the original attribute value, if appropriate
					boost::spirit::traits::post_transform(attr, attr_);
					return true;
				}

				// inform attribute transformation of failed rhs
				boost::spirit::traits::fail_transform(attr, attr_);
			}
			return false;
		}

		template <typename Context, typename Skipper
		  , typename Attribute, typename Params>
		bool parse(Iterator& first, Iterator const& last
		  , Context& caller_context, Skipper const& skipper
		  , Attribute& attr, Params const& params) const
		{
			if (f)
			{
				// do a preskip if this is an implied lexeme
				if (boost::is_same<skipper_type, boost::spirit::unused_type>::value)
					boost::spirit::qi::skip_over(first, last, skipper);

				typedef boost::spirit::traits::make_attribute<attr_type, Attribute> make_attribute;

				// do down-stream transformation, provides attribute for
				// rhs parser
				typedef boost::spirit::traits::transform_attribute<
					typename make_attribute::type, attr_type, boost::spirit::qi::domain> 
				transform;

				typename make_attribute::type made_attr = make_attribute::call(attr);
				typename transform::type attr_ = transform::pre(made_attr);

				// If you are seeing a compilation error here, you are probably
				// trying to use a rule or a grammar which has inherited
				// attributes, passing values of incompatible types for them.
				context_type context(attr_, params, caller_context);

				// If you are seeing a compilation error here stating that the
				// forth parameter can't be converted to a required target type
				// then you are probably trying to use a rule or a grammar with
				// an incompatible skipper type.
				if (f(first, last, context, skipper))
				{
					// do up-stream transformation, this integrates the results
					// back into the original attribute value, if appropriate
					boost::spirit::traits::post_transform(attr, attr_);
					return true;
				}

				// inform attribute transformation of failed rhs
				boost::spirit::traits::fail_transform(attr, attr_);
			}
			return false;
		}

		template <typename Context>
		info what(Context& /*context*/) const
		{
			return info(name_);
		}

		reference_ alias() const
		{
			return reference_(*this);
		}

		typename boost::proto::terminal<this_type>::type copy() const
		{
			typename boost::proto::terminal<this_type>::type result = {*this};
			return result;
		}

		// bring in the operator() overloads
		packrat_rule const& get_parameterized_subject() const { return *this; }
		typedef packrat_rule parameterized_subject_type;
		#include "detail/packrat_fcall.hpp"

		std::string name_;
		function_type f;
	};

#if BOOST_WORKAROUND(BOOST_MSVC, < 1400)
	template <typename OutputIterator_, typename T1_, typename T2_
	  , typename T3_, typename T4_, typename Expr>
	packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>& operator%=(
		packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>& r, Expr const& expr)
	{
		// Report invalid expression error as early as possible.
		// If you got an error_invalid_expression error message here,
		// then the expression (expr) is not a valid spirit qi expression.
		BOOST_SPIRIT_ASSERT_MATCH(boost::spirit::qi::domain, Expr);

		typedef typename 
			packrat_rule<OutputIterator_, T1_, T2_, T3_, T4_>::encoding_modifier_type
		encoding_modifier_type;

		r.f = boost::spirit::detail::bind_parser<mpl::true_>(
			boost::spirit::compile<boost::spirit::qi::domain>(detail::packratifier::meta_grammar()(expr, packratify), encoding_modifier_type()));
		return r;
	}

	template <typename Iterator_, typename T1_, typename T2_
	  , typename T3_, typename T4_, typename Expr>
	packrat_rule<Iterator_, T1_, T2_, T3_, T4_>& operator%=(
		packrat_rule<Iterator_, T1_, T2_, T3_, T4_>& r, Expr& expr)
	{
		return r %= static_cast<Expr const&>(expr);
	}
#endif

}}

namespace boost { namespace spirit { namespace traits
{
	///////////////////////////////////////////////////////////////////////////
	template <
		typename IteratorA, typename IteratorB, typename Attribute
	  , typename Context, typename T1, typename T2, typename T3, typename T4>
	struct handles_container<
		yak::spirit::packrat_rule<IteratorA, T1, T2, T3, T4>, Attribute, Context, IteratorB>
	  : traits::is_container<
		  typename attribute_of<
			  yak::spirit::packrat_rule<IteratorA, T1, T2, T3, T4>, Context, IteratorB
		  >::type
		>
	{};
}}}

namespace boost { namespace spirit { namespace qi {

	template <typename Subject>
	bool operator==(const reference<Subject> &arg1, const reference<Subject> &arg2)
	{
		return arg1.ref.get_pointer() == arg2.ref.get_pointer();
	}
	template <typename Subject>
	bool operator<(const reference<Subject> &arg1, const reference<Subject> &arg2)
	{
		// Usual pointer comparision is not defined between subobjects in different objects.
		return std::less<Subject*>()(arg1.ref.get_pointer(), arg2.ref.get_pointer());
	}
	template <typename Subject>
	std::size_t hash_value(const reference<Subject> &arg)
	{
		return boost::hash<Subject*>()(arg.ref.get_pointer());
	}

}}}

#endif
