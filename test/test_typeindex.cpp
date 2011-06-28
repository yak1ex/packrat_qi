/***********************************************************************/
/*                                                                     */
/* test_typeindex.cpp: Test code for type_index                        */
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

#include "typeindex.hpp"

#include <boost/test/auto_unit_test.hpp>

///////////////////////////////////////////////////////////////////////

struct A {};
struct B : A {};

BOOST_AUTO_TEST_CASE(test_static_type)
{
	B b;
	A *pa = &b;

	const std::type_info &info_pa = typeid(*pa);
	const std::type_info &info_b  = typeid(b);
	const std::type_info &info_A  = typeid(A);
	const std::type_info &info_B  = typeid(B);

	yak::util::type_index index_pa(info_pa);
	yak::util::type_index index_b (info_b);
	yak::util::type_index index_A (info_A);
	yak::util::type_index index_B (info_B);

	BOOST_CHECK( (index_pa == index_A));
	BOOST_CHECK( (index_A  == index_pa));
	BOOST_CHECK( (index_b  == index_B));
	BOOST_CHECK( (index_B  == index_b));
	BOOST_CHECK(!(index_A  == index_B));
	BOOST_CHECK(!(index_B  == index_A));

	BOOST_CHECK( (index_A != index_B));
	BOOST_CHECK( (index_B != index_A));
	BOOST_CHECK(!(index_A != index_A));
	BOOST_CHECK(!(index_B != index_B));

	BOOST_CHECK(!(index_A <  index_A));
	BOOST_CHECK(!(index_A >  index_A));
	BOOST_CHECK( (index_A <= index_A));
	BOOST_CHECK( (index_A >= index_A));

	BOOST_CHECK_EQUAL(((index_A < index_B) ^ (index_B < index_A)), 1);
	BOOST_CHECK_EQUAL(((index_A < index_B) ^ (index_A > index_B)), 1);
	BOOST_CHECK_EQUAL(((index_A > index_B) ^ (index_B > index_A)), 1);

	BOOST_CHECK_EQUAL((index_A < index_B), !(index_A >= index_B));
	BOOST_CHECK_EQUAL((index_A < index_A), !(index_A >= index_A));
	BOOST_CHECK_EQUAL((index_B < index_A), !(index_B >= index_A));
	BOOST_CHECK_EQUAL((index_B < index_B), !(index_B >= index_B));

	BOOST_CHECK_EQUAL((index_A > index_B), !(index_A <= index_B));
	BOOST_CHECK_EQUAL((index_A > index_A), !(index_A <= index_A));
	BOOST_CHECK_EQUAL((index_B > index_A), !(index_B <= index_A));
	BOOST_CHECK_EQUAL((index_B > index_B), !(index_B <= index_B));
}

struct C { virtual ~C() {} };
struct D : C {};

BOOST_AUTO_TEST_CASE(test_dynamic_type)
{
	D d, *pd = &d;
	C *pcd = &d, c, *pc = &c;

	const std::type_info &info_pc  = typeid(*pc);
	const std::type_info &info_pcd = typeid(*pcd);
	const std::type_info &info_pd  = typeid(*pd);
	const std::type_info &info_C   = typeid(C);
	const std::type_info &info_D   = typeid(D);

	yak::util::type_index index_pc (info_pc);
	yak::util::type_index index_pcd(info_pcd);
	yak::util::type_index index_pd (info_pd);
	yak::util::type_index index_C  (info_C);
	yak::util::type_index index_D  (info_D);

	BOOST_CHECK( (index_pc  == index_C));
	BOOST_CHECK( (index_pcd == index_D));
	BOOST_CHECK( (index_pd  == index_D));

}
