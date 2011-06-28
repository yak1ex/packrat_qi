/***********************************************************************/
/*                                                                     */
/* packrat_core.hpp: Packrat parser on Spirit - core part              */
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

#ifndef YAK_SPIRIT_PACKRAT_CORE_HPP
#define YAK_SPIRIT_PACKRAT_CORE_HPP

#include "fusion_hash.hpp"
#include "any_key.hpp"

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>

#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/include/support_attributes.hpp>
#include <boost/spirit/home/support/terminal.hpp>
#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/support/handles_container.hpp>

#include <boost/unordered_map.hpp>

#ifdef YAK_SPIRIT_PACKRAT_DEBUG_CLASS
#define YAK_SPIRIT_PACKRAT_DEBUG(func, arg) do { YAK_SPIRIT_PACKRAT_DEBUG_CLASS::func arg; } while(0)
#else
#define YAK_SPIRIT_PACKRAT_DEBUG(func, arg)
#endif

namespace yak { namespace spirit {

	BOOST_SPIRIT_TERMINAL(packratify)

	template<typename Iterator>
	struct packrat_hash
	{
		std::size_t operator()(const Iterator &iterator) const
		{
			boost::hash<typename std::iterator_traits<Iterator>::pointer> hasher;
			return hasher(&*iterator);
		}
	};

namespace detail {

	template<typename Subject, typename Context, typename Iterator>
	struct cache
	{
		typedef typename boost::spirit::traits::attribute_of<Subject, Context, Iterator>::type attr_type;
		typedef boost::tuple<const void*, yak::util::any_key> key_type;
		typedef boost::tuple<boost::any, Iterator> value_type;
		typedef boost::unordered_map<key_type, value_type, yak::fusion::hash > type;
		static type& call(Context &context)
		{
			return *context.table;
		}
	};

}

	template<typename Iterator, typename Attributes, typename Locals>
	struct context : boost::spirit::context<Attributes, Locals>
	{
		typedef boost::spirit::context<Attributes, Locals> base_type;
		typedef boost::tuple<const void*, yak::util::any_key> key_type;
		typedef boost::tuple<boost::any, Iterator> value_type;
		typedef boost::unordered_map<key_type, value_type, yak::fusion::hash > cache_type;

		template<typename Context>
		context(typename Attributes::car_type attribute, Context& caller_context)
		  : base_type(attribute), table(new cache_type) {}

		context(typename Attributes::car_type attribute, context& caller_context)
		  : base_type(attribute), table(caller_context.table) {}

		template <typename Args, typename Context>
		context(
			typename Attributes::car_type attribute
		  , Args const& args
		  , Context& caller_context
		) : base_type(attribute, args, caller_context), table(new cache_type) {}

		template <typename Args>
		context(
			typename Attributes::car_type attribute
		  , Args const& args
		  , context& caller_context // inheritance check might be better
		) : base_type(attribute, args, caller_context), table(caller_context.table) {}

		context(Attributes const& attributes) : base_type(attributes), table(new cache_type) {}

		boost::shared_ptr<cache_type> table;
	};

	template<typename Subject>
	struct packrat_parser
		: boost::spirit::qi::unary_parser<packrat_parser<Subject> >
	{
		typedef Subject subject_type;

		template<typename Context, typename Iterator>
		struct attribute
		{
			typedef typename boost::spirit::traits::attribute_of<
				Subject, Context, Iterator
			>::type type;
		};

		packrat_parser(Subject const &subject) : subject(subject) {}

//		It might be better to include type of Attribute in key_type
		template<typename Iterator, typename Context, typename Skipper, typename Attribute>
		bool parse(Iterator &first, Iterator const &last,
			Context &context, Skipper const &skipper, Attribute &attribute) const
		{
			typedef typename boost::spirit::traits::attribute_of<Subject, Context, Iterator>::type attr_type;
			typedef typename detail::cache<Subject, Context, Iterator>::type cache_type;
			typedef typename detail::cache<Subject, Context, Iterator>::key_type key_type;
			typedef typename detail::cache<Subject, Context, Iterator>::value_type value_type;

			cache_type &table = detail::cache<Subject, Context, Iterator>::call(context);
			key_type key(static_cast<const void*>(&*first), yak::util::any_key(subject));
#ifdef YAK_SPIRIT_PACKRAT_DEBUG_CLASS
			std::size_t position = std::distance(first, last);
			std::size_t hash_code = boost::get<1>(key).hash_code();
#endif
			YAK_SPIRIT_PACKRAT_DEBUG(before, (position, hash_code, table.count(key), table.size()));
			if(table.count(key)) {
				if(!boost::get<0>(table[key]).empty()) {
					boost::spirit::traits::assign_to(boost::any_cast<attr_type>(boost::get<0>(table[key])), attribute);
					first = boost::get<1>(table[key]);
					YAK_SPIRIT_PACKRAT_DEBUG(hit_success, (position, hash_code, &typeid(attr_type), &typeid(attribute), attribute));
				} else {
					YAK_SPIRIT_PACKRAT_DEBUG(hit_fail, (position, hash_code));
				}
				return !boost::get<0>(table[key]).empty();
			}
			attr_type attr = attr_type();
			bool result = subject.parse(first, last, context, skipper, attr);
			if(result) {
				boost::get<0>(table[key]) = attr;
				boost::get<1>(table[key]) = first;
				boost::spirit::traits::assign_to(attr, attribute);
				YAK_SPIRIT_PACKRAT_DEBUG(parse_success, (position, hash_code, &typeid(attr_type), &typeid(attribute), attribute));
			} else {
				boost::get<0>(table[key]) = boost::any();
				YAK_SPIRIT_PACKRAT_DEBUG(parse_fail, (position, hash_code));
			}
			YAK_SPIRIT_PACKRAT_DEBUG(after, (position, hash_code, table.count(key), table.size()));
			return result;
		}

		template <typename Context>
		boost::spirit::info what(Context& context) const
		{
			return boost::spirit::info("packrat", subject.what(context));
		}

		Subject subject;

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        packrat_parser& operator= (packrat_parser const&);

	};

	template<typename Subject>
	inline bool operator==(const packrat_parser<Subject>& arg1, const packrat_parser<Subject>& arg2)
	{
		return arg1.subject == arg2.subject;
	}
	template<typename Subject>
	inline bool operator<(const packrat_parser<Subject>& arg1, const packrat_parser<Subject>& arg2)
	{
		return arg1.subject < arg2.subject;
	}
	template<typename Subject>
	inline std::size_t hash_value(const packrat_parser<Subject>& arg)
	{
		return boost::hash<Subject>()(arg.subject);
	}

}}


namespace boost { namespace spirit { namespace qi {

	template <typename Subject, typename Modifiers>
	struct make_directive<
		yak::spirit::tag::packratify, Subject, Modifiers>
	{
		typedef yak::spirit::packrat_parser<Subject> result_type;

		result_type operator()(unused_type, Subject const& subject, unused_type) const
		{
			return result_type(subject);
		}
	};

}}}

namespace boost { namespace spirit {

// Enabler
	template<>
	struct use_directive<qi::domain, yak::spirit::tag::packratify> : mpl::true_ {};

}}

namespace boost { namespace spirit { namespace traits
{
	template <typename Subject>
	struct has_semantic_action<yak::spirit::packrat_parser<Subject> >
	  : unary_has_semantic_action<Subject> {};

	template <typename Subject, typename Attribute, typename Context, typename Iterator>
	struct handles_container<yak::spirit::packrat_parser<Subject>, Attribute, Context, Iterator>
	  : unary_handles_container<Subject, Attribute, Context, Iterator> {};

}}}

#endif
