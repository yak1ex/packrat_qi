/***********************************************************************/
/*                                                                     */
/* any_key.hpp: Any key type holder for map / unordered_map            */
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

#ifndef YAK_UTIL_ANY_KEY_HPP
#define YAK_UTIL_ANY_KEY_HPP

#include "typeindex.hpp"
#include "fusion_hash.hpp"

namespace yak { namespace util {

	class any_key
	{
	private:

		struct placeholder
		{
			virtual bool operator==(const any_key& k) const = 0;
			virtual bool operator< (const any_key& k) const = 0;
			virtual placeholder* clone()              const = 0;
			virtual const std::type_info& type()      const = 0;
			virtual std::size_t hash_code()           const = 0;
			virtual ~placeholder(){}
		};

		template<typename T>
		class holder : public placeholder
		{
		public:
			holder(const T& t) : t(t) {}
			bool operator==(const any_key&k) const
			{
				return typeindex() == k.typeindex() && t == to_mytype(k);
			}
			bool operator< (const any_key&k) const
			{
				return typeindex() < k.typeindex() || (typeindex() == k.typeindex() && t < to_mytype(k));
			}
			placeholder* clone() const { return new holder(t); }
			const std::type_info& type() const { return typeid(T); }
			type_index typeindex() const { return type_index(type()); }
			std::size_t hash_code() const
			{
				std::size_t hv = typeindex().hash_code();
				// boost::hash_combine with Fusion sequence support
				yak::util::hash_combine(hv, t);
				return hv;
			}
		private:
			static const T& to_mytype(const any_key& k)
			{
				return static_cast<holder<T>*>(k.p)->t;
			}
			T t;
		};

		placeholder *p;

	public:

		any_key() : p(0){}
		template<typename T>
		any_key(const T& t) : p(new holder<T>(t)) {}
		any_key(const any_key& k) : p(k.p->clone()) {}
		any_key& operator= (any_key k)
		{
			swap(k);
			return *this;
		}
		template<typename T>
		any_key& operator= (const T& t)
		{
			any_key k(t);
			swap(k);
			return *this;
		}
		~any_key() { delete p; }
		void swap(any_key& k) { std::swap(p, k.p); }
		bool operator==(const any_key& k) const
		{
			return p->operator==(k);
		}
		bool operator!=(const any_key& k) const
		{
			return !p->operator==(k);
		}
		bool operator< (const any_key& k) const
		{
			return p->operator<(k);
		}
		bool operator<=(const any_key& k) const
		{
			return !k.p->operator<(*this);
		}
		bool operator> (const any_key& k) const
		{
			return k.p->operator<(*this);
		}
		bool operator>=(const any_key& k) const
		{
			return !p->operator<(k);
		}
		const std::type_info& type() const
		{
			return p->type();
		}
		type_index typeindex() const
		{
			return type_index(p->type());
		}
		std::size_t hash_code() const
		{
			return p->hash_code();
		}
	};
	inline std::size_t hash_value(const any_key& k)
	{
		return k.hash_code();
	}

}}

#endif
