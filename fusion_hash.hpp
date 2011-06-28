/***********************************************************************/
/*                                                                     */
/* fusion_hash.hpp: Hash functions for Fusion sequence                 */
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

#ifndef YAK_FUSION_HASH_HPP
#define YAK_FUSION_HASH_HPP

#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/functional/hash.hpp>
#include <boost/utility/enable_if.hpp>

namespace yak { namespace util {

template<typename T>
inline typename boost::disable_if<boost::fusion::traits::is_sequence<T> >::type
hash_combine(std::size_t &seed, const T& t);

template<typename T>
inline typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type
hash_combine(std::size_t &seed, const T& t);

}}

namespace yak { namespace fusion {

namespace detail {

	struct hasher
	{
		typedef std::size_t result_type;
		template<typename Arg>
		std::size_t operator()(const std::size_t& seed, const Arg& arg) const
		{
			std::size_t seed_ = seed;
			yak::util::hash_combine(seed_, arg);
			return seed_;
		}
	};

}

struct hash
{
	typedef std::size_t result_type;
	template<typename FSeq>
	std::size_t operator()(const FSeq &fv, std::size_t seed = 0) const
	{
		return boost::fusion::fold(fv, seed, detail::hasher());
	}
};

}}

namespace yak { namespace util {

template<typename T>
inline typename boost::disable_if<boost::fusion::traits::is_sequence<T> >::type
hash_combine(std::size_t &seed, const T& t)
{
	boost::hash_combine(seed, t);
}

template<typename T>
inline typename boost::enable_if<boost::fusion::traits::is_sequence<T> >::type
hash_combine(std::size_t &seed, const T& t)
{
	seed = yak::fusion::hash()(t, seed);
}

}}


#endif
