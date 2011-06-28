/***********************************************************************/
/*                                                                     */
/* typeindex.hpp: Backport for std::type_index in C++0x                */
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

#ifndef YAK_UTIL_TYPEINDEX_HPP
#define YAK_UTIL_TYPEINDEX_HPP

#include <cstddef>
#include <typeinfo>

#include <boost/functional/hash.hpp>

namespace yak { namespace util {

	class type_index
	{
	public:
		type_index(const std::type_info& rhs) : target(&rhs) {}
		bool operator==(const type_index& rhs) const { return *target == *rhs.target; }
		bool operator!=(const type_index& rhs) const { return *target != *rhs.target; }
		bool operator< (const type_index& rhs) const { return target->before(*rhs.target); }
		bool operator<=(const type_index& rhs) const { return !rhs.target->before(*target); }
		bool operator> (const type_index& rhs) const { return rhs.target->before(*target); }
		bool operator>=(const type_index& rhs) const { return !target->before(*rhs.target); }
		std::size_t hash_code() const { return boost::hash<const std::type_info*>()(target); }
		const char* name() const { return target->name(); }
	private:
		const std::type_info* target;
	};

}}

#endif
