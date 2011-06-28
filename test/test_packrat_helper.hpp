/***********************************************************************/
/*                                                                     */
/* test_packrat_helper.hpp: Test code helper for Packrat parser        */
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

#ifndef YAK_SPIRIT_PACKRAT_TEST_PACKRAT_HELPER_HPP
#define YAK_SPIRIT_PACKRAT_TEST_PACKRAT_HELPER_HPP

#include <cstddef>

#include <iostream>
#include <sstream>
#include "any_key.hpp"

namespace yak { namespace spirit { namespace debug {

	class trace
	{
	public:
		static bool enable;
		static void begin() {}
		static void end() {}
		static void before(std::ptrdiff_t pos, std::size_t hash_code, std::size_t hit, std::size_t size)
		{
			if(!enable) return;
			std::cerr << "[before] position: " << pos << " : " << hash_code << std::endl;
			std::cerr << "[before] table.count(key): " << hit << std::endl;
			std::cerr << "[before] table.size(): " << size << std::endl;
		}
		static void after(std::ptrdiff_t pos, std::size_t hash_code, std::size_t hit, std::size_t size)
		{
			if(!enable) return;
			std::cerr << "[after] position: " << pos << " : " << hash_code << std::endl;
			std::cerr << "[after] table.count(key): " << hit << std::endl;
			std::cerr << "[after] table.size(): " << size << std::endl;
		}
		static void hit_success(std::ptrdiff_t pos, std::size_t hash_code, const std::type_info* type_from, const std::type_info* type_to, const yak::util::any_key& k)
		{
			if(!enable) return;
			std::cerr << "[hit_success] position: " << pos << " : " << hash_code << std::endl;
			std::cerr << "[hit_success] type_from: " << type_from->name() << " type_to: " << type_to->name() << std::endl;
		}
		static void hit_fail(std::ptrdiff_t pos, std::size_t hash_code)
		{
			if(!enable) return;
			std::cerr << "[hit_fail] position: " << pos << " : " << hash_code << std::endl;
		}
		static void parse_success(std::ptrdiff_t pos, std::size_t hash_code, const std::type_info* type_from, const std::type_info* type_to, const yak::util::any_key &k)
		{
			if(!enable) return;
			std::cerr << "[parse_success] position: " << pos << " : " << hash_code << std::endl;
			std::cerr << "[parse_success] type_from: " << type_from->name() << " type_to: " << type_to->name() << std::endl;
		}
		static void parse_fail(std::ptrdiff_t pos, std::size_t hash_code)
		{
			if(!enable) return;
			std::cerr << "[parse_fail] position: " << pos << " : " << hash_code << std::endl;
		}
		struct enabler
		{
			enabler(bool = true) { begin(); enable = true; }
			~enabler() { enable = false; }
			operator bool() { return true; }
		};
	};

	class count
	{
	public:
		static bool enable;
		static void begin()
		{
			hit_success_ = hit_fail_ = parse_success_ = parse_fail_ = 0;
		}
		static void end() {}
		static void before(std::ptrdiff_t pos, std::size_t hash_code, std::size_t hit, std::size_t size)
		{}
		static void after(std::ptrdiff_t pos, std::size_t hash_code, std::size_t hit, std::size_t size)
		{}
		static void hit_success(std::ptrdiff_t pos, std::size_t hash_code, const std::type_info* type_from, const std::type_info* type_to, const yak::util::any_key& k)
		{
			if(!enable) return;
			++hit_success_;
		}
		static void hit_fail(std::ptrdiff_t pos, std::size_t hash_code)
		{
			if(!enable) return;
			++hit_fail_;
		}
		static void parse_success(std::ptrdiff_t pos, std::size_t hash_code, const std::type_info* type_from, const std::type_info* type_to, const yak::util::any_key &k)
		{
			if(!enable) return;
			++parse_success_;
		}
		static void parse_fail(std::ptrdiff_t pos, std::size_t hash_code)
		{
			if(!enable) return;
			++parse_fail_;
		}
		static void out(std::ostream &os)
		{
			os << "hit_success:  " << hit_success_ << std::endl;
			os << "hit_fail:     " << hit_fail_ << std::endl;
			os << "parse_sucess: " << parse_success_ << std::endl;
			os << "parse_fail:   " << parse_fail_ << std::endl;
		}
		static int hit_success_;
		static int hit_fail_;
		static int parse_success_;
		static int parse_fail_;
		struct enabler
		{
			enabler(bool = true) { begin(); enable = true; }
			~enabler() { enable = false; }
			operator bool() { return true; }
		};
	};

	template<typename New, typename Base>
	struct combine
	{
		struct enabler
		{
			enabler(bool = true)
			{
				begin();
				New::enable = Base::enable = true;
			}
			~enabler()
			{
				New::enable = Base::enable = false;
			}
			operator bool() { return true; }
		};
		static void begin()
		{	New::begin();	Base::begin();	}
		static void end()
		{	New::end();		Base::end();	}
		static void before(std::ptrdiff_t pos, std::size_t hash_code, std::size_t hit, std::size_t size)
		{
			New::before(pos, hash_code, hit, size);
			Base::before(pos, hash_code, hit, size);
		}
		static void after(std::ptrdiff_t pos, std::size_t hash_code, std::size_t hit, std::size_t size)
		{
			New::after(pos, hash_code, hit, size);
			Base::after(pos, hash_code, hit, size);
		}
		static void hit_success(std::ptrdiff_t pos, std::size_t hash_code, const std::type_info* type_from, const std::type_info* type_to, const yak::util::any_key& k)
		{
			New::hit_success(pos, hash_code, type_from, type_to, k);
			Base::hit_success(pos, hash_code, type_from, type_to, k);
		}
		static void hit_fail(std::ptrdiff_t pos, std::size_t hash_code)
		{
			New::hit_fail(pos, hash_code);
			Base::hit_fail(pos, hash_code);
		}
		static void parse_success(std::ptrdiff_t pos, std::size_t hash_code, const std::type_info* type_from, const std::type_info* type_to, const yak::util::any_key &k)
		{
			New::parse_success(pos, hash_code, type_from, type_to, k);
			Base::parse_success(pos, hash_code, type_from, type_to, k);
		}
		static void parse_fail(std::ptrdiff_t pos, std::size_t hash_code)
		{
			New::parse_fail(pos, hash_code);
			Base::parse_fail(pos, hash_code);
		}
	};

	extern std::stringstream test_ss;

}}}

#define BOOST_SPIRIT_DEBUG_OUT yak::spirit::debug::test_ss
#define YAK_SPIRIT_PACKRAT_DEBUG_CLASS yak::spirit::debug::count

#define YAK_SPIRIT_PACKRAT_TEST_BLOCK if(YAK_SPIRIT_PACKRAT_DEBUG_CLASS::enabler enabler ## __FILE__ ## __LINE__ = true)

#endif
