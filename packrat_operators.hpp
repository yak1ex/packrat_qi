/***********************************************************************/
/*                                                                     */
/* packrat_operators.hpp: Packrat parser on Spirit - operators part    */
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

#ifndef YAK_SPIRIT_PACKRAT_OPERATORS_HPP
#define YAK_SPIRIT_PACKRAT_OPERATORS_HPP

#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/detail/endian.hpp>

#include <boost/fusion/include/comparison.hpp>

namespace boost { namespace spirit {

	inline bool operator==(const unused_type&, const unused_type&)
	{
		return true;
	}
	inline bool operator<(const unused_type&, const unused_type&)
	{
		return false;
	}
	inline std::size_t hash_value(const unused_type&)
	{
		return 0;
	}

namespace qi {

///////////////////////////////////////////////////////////////////////
//
// for Primitive Parser
//

	struct semantic_predicate;

	template<typename PrimitiveParser>
	inline typename boost::enable_if<
		boost::mpl::and_<
			boost::is_base_of<boost::spirit::qi::primitive_parser<PrimitiveParser>, PrimitiveParser>, 
			boost::mpl::not_<boost::is_same<boost::spirit::qi::semantic_predicate, PrimitiveParser> >
		>,
		bool
	>::type
	operator==(const PrimitiveParser& arg1, const PrimitiveParser& arg2)
	{
		return true;
	}
	template<typename PrimitiveParser>
	inline typename boost::enable_if<
		boost::mpl::and_<
			boost::is_base_of<boost::spirit::qi::primitive_parser<PrimitiveParser>, PrimitiveParser>, 
			boost::mpl::not_<boost::is_same<boost::spirit::qi::semantic_predicate, PrimitiveParser> >
		>,
		bool
	>::type
	operator<(const PrimitiveParser& arg1, const PrimitiveParser& arg2)
	{
		return false;
	}
	template<typename PrimitiveParser>
	inline typename boost::enable_if<
		boost::mpl::and_<
			boost::is_base_of<boost::spirit::qi::primitive_parser<PrimitiveParser>, PrimitiveParser>, 
			boost::mpl::not_<boost::is_same<boost::spirit::qi::semantic_predicate, PrimitiveParser> >
		>,
		std::size_t
	>::type
	hash_value(const PrimitiveParser& arg)
	{
		return 0;
	}

///////////////////////////////////////////////////////////////////////
//
// for Unary Parser
//

	template<typename UnaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::unary_parser<UnaryParser>, UnaryParser>, bool>::type
	operator==(const UnaryParser& arg1, const UnaryParser& arg2)
	{ // Existence of subject is NOT an official requirement.
		return arg1.subject == arg2.subject;
	}
	template<typename UnaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::unary_parser<UnaryParser>, UnaryParser>, bool>::type
	operator<(const UnaryParser& arg1, const UnaryParser& arg2)
	{ // Existence of subject is NOT an official requirement.
		return arg1.subject < arg2.subject;
	}
	template<typename UnaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::unary_parser<UnaryParser>, UnaryParser>, std::size_t>::type
	hash_value(const UnaryParser& arg)
	{ // Existence of subject and subject_type is NOT an official requirement.
		boost::hash<typename UnaryParser::subject_type> hasher;
		return hasher(arg.subject);
	}

///////////////////////////////////////////////////////////////////////
//
// for Binary Parser
//

	template<typename BinaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::binary_parser<BinaryParser>, BinaryParser>, bool>::type
	operator==(const BinaryParser& arg1, const BinaryParser& arg2)
	{ // Existence of left and right is NOT an official requirement.
		return arg1.left == arg2.left && arg1.right == arg2.right;
	}
	template<typename BinaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::binary_parser<BinaryParser>, BinaryParser>, bool>::type
	operator<(const BinaryParser& arg1, const BinaryParser& arg2)
	{ // Existence of left and right is NOT an official requirement.
		return arg1.left < arg2.left || (arg1.left == arg2.left && arg1.right < arg2.right);
	}
	template<typename BinaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::binary_parser<BinaryParser>, BinaryParser>, std::size_t>::type
	hash_value(const BinaryParser& arg)
	{ // Existence of left and right is NOT an official requirement.
		std::size_t seed = 0;
		boost::hash_combine(seed, arg.left);
		boost::hash_combine(seed, arg.right);
		return seed;
	}

///////////////////////////////////////////////////////////////////////
//
// for Nary Parser
//

	template<typename NaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::nary_parser<NaryParser>, NaryParser>, bool>::type
	operator==(const NaryParser& arg1, const NaryParser& arg2)
	{ // Existence of elements is NOT an official requirement.
		return boost::fusion::operator==(arg1.elements, arg2.elements);
	}
	template<typename NaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::nary_parser<NaryParser>, NaryParser>, bool>::type
	operator<(const NaryParser& arg1, const NaryParser& arg2)
	{ // Existence of elements is NOT an official requirement.
		return boost::fusion::operator<(arg1.elements, arg2.elements);
	}
	template<typename NaryParser>
	inline typename boost::enable_if<boost::is_base_of<boost::spirit::qi::nary_parser<NaryParser>, NaryParser>, std::size_t>::type
	hash_value(const NaryParser& arg)
	{ // Existence of elements is NOT an official requirement.
		return yak::fusion::hash()(arg.elements);
	}

}}}

#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/seq/subseq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

namespace boost { namespace spirit { namespace qi {

///////////////////////////////////////////////////////////////////////
//
// for specific parsers
//

#define YAK_SPIRIT_PACKRAT_OPERATORS_OP1(s, data, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem)
#define YAK_SPIRIT_PACKRAT_OPERATORS_OP2(s, data, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem)

#define YAK_SPIRIT_PACKRAT_OPERATORS_OP3(s, data, elem) arg1.elem == arg2.elem
#define YAK_SPIRIT_PACKRAT_OPERATORS_OP4(r, data, i, elem) elem BOOST_PP_IF(BOOST_PP_EQUAL(data, BOOST_PP_INC(i)), , &&)

#define YAK_SPIRIT_PACKRAT_OPERATORS_OP5(s, data, elem) arg1.elem < arg2.elem
#define YAK_SPIRIT_PACKRAT_OPERATORS_OP6(r, data, i, elem) BOOST_PP_IF(BOOST_PP_EQUAL(data, BOOST_PP_INC(i)), YAK_SPIRIT_PACKRAT_OPERATORS_OP5, YAK_SPIRIT_PACKRAT_OPERATORS_OP3)(_, _, elem) BOOST_PP_IF(BOOST_PP_EQUAL(data, BOOST_PP_INC(i)), , &&)
#define YAK_SPIRIT_PACKRAT_OPERATORS_OP7(z, n, data) (BOOST_PP_SEQ_FOR_EACH_I(YAK_SPIRIT_PACKRAT_OPERATORS_OP6, BOOST_PP_INC(n), BOOST_PP_SEQ_SUBSEQ(data, 0, BOOST_PP_INC(n)))) BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(data), BOOST_PP_INC(n)), , ||)

#define YAK_SPIRIT_PACKRAT_OPERATORS_OP8(s, data, elem) boost::hash_combine(seed, arg.elem);

#define YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(type, num, seq_type, seq_elem) \
	template <BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP1, _, BOOST_PP_SEQ_SUBSEQ(seq_type, 0, num)))> \
	inline bool operator==(const type<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP2, _, seq_type))>& arg1, const type<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP2, _, seq_type))>& arg2) \
	{ \
		return BOOST_PP_SEQ_FOR_EACH_I(YAK_SPIRIT_PACKRAT_OPERATORS_OP4, BOOST_PP_SEQ_SIZE(seq_elem), BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP3, _, seq_elem)); \
	} \
	template <BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP1, _, BOOST_PP_SEQ_SUBSEQ(seq_type, 0, num)))> \
	inline bool operator<(const type<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP2, _, seq_type))>& arg1, const type<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP2, _, seq_type))>& arg2) \
	{ \
		return BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(seq_elem), YAK_SPIRIT_PACKRAT_OPERATORS_OP7, seq_elem); \
	} \
	template <BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP1, _, BOOST_PP_SEQ_SUBSEQ(seq_type, 0, num)))> \
	inline std::size_t hash_value(const type<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(YAK_SPIRIT_PACKRAT_OPERATORS_OP2, _, seq_type))>& arg) \
	{ \
		std::size_t seed = 0;\
		BOOST_PP_SEQ_FOR_EACH(YAK_SPIRIT_PACKRAT_OPERATORS_OP8, _, seq_elem)\
		return seed; \
	}

// for character parsers

	template <typename CharEncoding, bool no_attribute, bool no_case>
	struct literal_char;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_char, 2, ((typename, CharEncoding))((bool, no_attribute))((bool, false)), (ch))
	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_char, 2, ((typename, CharEncoding))((bool, no_attribute))((bool, true)), (lo)(hi))

	template <typename CharEncoding, bool no_case>
	struct char_range;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(char_range, 1, ((typename, CharEncoding))((bool, false)), (from)(to))
	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(char_range, 1, ((typename, CharEncoding))((bool, true)), (from_lo)(to_lo)(from_hi)(to_hi))

} namespace support { namespace detail {

	template <typename Char>
	struct basic_chset_8bit;

	template <typename Char>
	inline bool operator==(const basic_chset_8bit<Char>& arg1, const basic_chset_8bit<Char>& arg2)
	{
		for(int i = std::numeric_limits<Char>::min(); i <= std::numeric_limits<Char>::max(); ++i) {
			if(arg1.test(i) ^ arg2.test(i)) return false;
		}
		return true;
	}
	template <typename Char>
	inline bool operator<(const basic_chset_8bit<Char>& arg1, const basic_chset_8bit<Char>& arg2)
	{
		for(int i = std::numeric_limits<Char>::min(); i <= std::numeric_limits<Char>::max(); ++i) {
			if(!arg1.test(i) && arg2.test(i)) return true;
			if(arg1.test(i) && !arg2.test(i)) return false;
		}
		return false;
	}
	template <typename Char>
	inline std::size_t hash_value(const basic_chset_8bit<Char>& arg)
	{
		std::size_t seed = 0;
		for(int i = std::numeric_limits<Char>::min(); i <= std::numeric_limits<Char>::max(); ++i) {
			boost::hash_combine(seed, arg.test(i));
		}
		return seed;
	}

}} namespace qi {

	template <typename CharEncoding, bool no_attribute, bool no_case>
	struct char_set;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(char_set, 2, ((typename, CharEncoding))((bool, no_attribute))((bool, false)), (chset))
	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(char_set, 2, ((typename, CharEncoding))((bool, no_attribute))((bool, true)), (chset))

// for real parsers

	template <typename T, typename RealPolicies, bool no_attribute>
	struct literal_real_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_real_parser, 3, ((typename, T))((typename, RealPolicies))((bool, no_attribute)), (n_))

// for int parsers

	template <typename T, unsigned Radix, unsigned MinDigits, int MaxDigits, bool no_attribute>
	struct literal_int_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_int_parser, 5, ((typename, T))((unsigned, Radix))((unsigned, MinDigits))((int, MaxDigits))((bool, no_attribute)), (n_))

// for uint parsers

	template <typename T, unsigned Radix, unsigned MinDigits, int MaxDigits, bool no_attribute>
	struct literal_uint_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_uint_parser, 5, ((typename, T))((unsigned, Radix))((unsigned, MinDigits))((int, MaxDigits))((bool, no_attribute)), (n_))

// for bool parsers

	template <typename T, typename BoolPolicies, bool no_attribute>
	struct literal_bool_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_bool_parser, 3, ((typename, T))((typename, BoolPolicies))((bool, no_attribute)), (n_))

// for string parsers

	template <typename String, bool no_attribute>
	struct literal_string;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(literal_string, 2, ((typename, String))((bool, no_attribute)), (str))

	template <typename String, bool no_attribute>
	struct no_case_literal_string;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(no_case_literal_string, 2, ((typename, String))((bool, no_attribute)), (str_lo)(str_hi))

	template <typename Char, typename T, typename Lookup, typename Filter>
	struct symbols;

	template <typename Subject>
	struct reference;

	template <typename Char, typename T, typename Lookup, typename Filter>
	inline bool operator==(const reference<symbols<Char, T, Lookup, Filter> > &arg1, const reference<symbols<Char, T, Lookup, Filter> > &arg2)
	{
		return arg1.ref.get().lookup.get() == arg2.ref.get().lookup.get();
	}
	template <typename Char, typename T, typename Lookup, typename Filter>
	inline bool operator<(const reference<symbols<Char, T, Lookup, Filter> > &arg1, const reference<symbols<Char, T, Lookup, Filter> > &arg2)
	{
		// Usual pointer comparision is not defined between subobjects in different objects.
		return std::less<Lookup*>()(arg1.ref.get().lookup.get(), arg2.ref.get().lookup.get());
	}
	template <typename Char, typename T, typename Lookup, typename Filter>
	inline std::size_t hash_value(const reference<symbols<Char, T, Lookup, Filter> > &arg)
	{
		return boost::hash<Lookup*>()(arg.ref.get().lookup.get());
	}

// for aux parsers

//	struct semantic_predicate;

	// Using template to avoid necessity of inclusion of definition
	template<typename T>
	inline typename boost::enable_if<boost::is_same<T, semantic_predicate>, bool>::type
	operator==(const T& arg1, const T& arg2)
	{
		return arg1.predicate == arg2.predicate;
	}
	template<typename T>
	inline typename boost::enable_if<boost::is_same<T, semantic_predicate>, bool>::type
	operator<(const T& arg1, const T& arg2)
	{
		return arg1.predicate < arg2.predicate;
	}
	template<typename T>
	inline typename boost::enable_if<boost::is_same<T, semantic_predicate>, std::size_t>::type
	hash_value(const T& arg)
	{
		return boost::hash<bool>()(arg.predicate);
	}

//	attr_cast parser

	template <typename Exposed, typename Transformed, typename Subject>
	struct attr_cast_parser;

	template <typename Exposed, typename Transformed, typename Subject>
	bool operator==(const attr_cast_parser<Exposed, Transformed, Subject>& arg1, const attr_cast_parser<Exposed, Transformed, Subject>& arg2)
	{
		return compile<qi::domain>(arg1.subject) == compile<qi::domain>(arg2.subject);
	}
	template <typename Exposed, typename Transformed, typename Subject>
	bool operator<(const attr_cast_parser<Exposed, Transformed, Subject>& arg1, const attr_cast_parser<Exposed, Transformed, Subject>& arg2)
	{
		return compile<qi::domain>(arg1.subject) < compile<qi::domain>(arg2.subject);
	}
	template <typename Exposed, typename Transformed, typename Subject>
	std::size_t hash_value(const attr_cast_parser<Exposed, Transformed, Subject>& arg)
	{
		return boost::hash<typename attr_cast_parser<Exposed, Transformed, Subject>::subject_type>()(compile<qi::domain>(arg.subject));
	}

	template <typename Value>
	struct attr_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(attr_parser, 1, ((typename, Value)), (value_))

//	binary parser

	template <typename Int, BOOST_SCOPED_ENUM(boost::integer::endianness) endian, int bits>
	struct binary_lit_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(binary_lit_parser, 3, ((typename, Int))((BOOST_SCOPED_ENUM(boost::integer::endianness), endian))((int, bits)), (n))

//	directive

	template <typename T>
	struct exact_iterator;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(exact_iterator, 1, ((typename, T)), (exact))

	template <typename T>
	struct finite_iterator;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(finite_iterator, 1, ((typename, T)), (min)(max))

	template <typename T>
	struct infinite_iterator;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(infinite_iterator, 1, ((typename, T)), (min))

	template <typename Subject, typename LoopIter>
	struct repeat_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(repeat_parser, 2, ((typename, Subject))((typename, LoopIter)), (subject)(iter))

	template <typename Subject, typename Skipper>
	struct skip_parser;

	YAK_SPIRIT_PACKRAT_DEFINE_OPERATORS(skip_parser, 2, ((typename, Subject))((typename, Skipper)), (subject)(skipper))

}}}

#endif
