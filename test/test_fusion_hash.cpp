/***********************************************************************/
/*                                                                     */
/* test_fusion_hash.cpp: Test code for hash function for Fusion seq.   */
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

#include "fusion_hash.hpp"

#include <boost/test/auto_unit_test.hpp>

#include <boost/fusion/include/make_vector.hpp>

#include <boost/array.hpp>
#include <boost/fusion/include/boost_array.hpp>

#include <boost/fusion/include/iterator_range.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/prior.hpp>

BOOST_AUTO_TEST_CASE(test_vector)
{
	using yak::fusion::hash;
	hash hasher;

	using boost::fusion::make_vector;

	BOOST_CHECK_EQUAL(hasher(make_vector(1)), hasher(make_vector(1)));
	BOOST_CHECK_EQUAL(hasher(make_vector(1, 3.5)), hasher(make_vector(1, 3.5)));
	BOOST_CHECK_EQUAL(hasher(make_vector(1, 3.5, std::string("hoge"))), hasher(make_vector(1, 3.5, std::string("hoge"))));

	BOOST_WARN_NE(hasher(make_vector(1)), hasher(make_vector(2)));
	BOOST_WARN_NE(hasher(make_vector(1)), hasher(make_vector(1, 3.5)));
	BOOST_WARN_NE(hasher(make_vector(1)), hasher(make_vector(1, 3.5, std::string("hoge"))));
}

BOOST_AUTO_TEST_CASE(test_array)
{
	using yak::fusion::hash;
	hash hasher;

	boost::array<int ,3> arr1 = {1, 2, 3};
	boost::array<int ,3> arr2 = {1, 2, 3};
	boost::array<int ,4> arr3 = {1, 2, 3, 4};
	boost::array<int ,4> arr4 = {1, 2, 3, 4};

	BOOST_CHECK_EQUAL(hasher(arr1), hasher(arr1));
	BOOST_CHECK_EQUAL(hasher(arr1), hasher(arr2));
	BOOST_CHECK_EQUAL(hasher(arr3), hasher(arr4));
	BOOST_CHECK_EQUAL(hasher(arr3), hasher(arr3));

	BOOST_WARN_NE(hasher(arr1), hasher(arr3));
	BOOST_WARN_NE(hasher(arr2), hasher(arr4));
}

BOOST_AUTO_TEST_CASE(test_iterator_range)
{
	using yak::fusion::hash;
	hash hasher;

	typedef boost::fusion::vector<int, char, double> v1_type;
	typedef boost::fusion::vector<int, char, double, std::string> v2_type;

	v1_type v1(1, 'c', 3.5);
	v2_type v2(1, 'c', 3.5, std::string("hoge"));

	typedef boost::fusion::result_of::begin<v1_type>::type v10_type;
	typedef boost::fusion::result_of::end<v1_type>::type v13_type;
	typedef boost::fusion::result_of::begin<v2_type>::type v20_type;
	typedef boost::fusion::result_of::end<v2_type>::type v24_type;
	typedef boost::fusion::result_of::prior<v24_type>::type v23_type;

	v10_type v10(boost::fusion::begin(v1));
	v13_type v13(boost::fusion::end(v1));
	v20_type v20(boost::fusion::begin(v2));
	v24_type v24(boost::fusion::end(v2));
	v23_type v23(boost::fusion::prior(v24));

	BOOST_CHECK_EQUAL(hasher(boost::fusion::iterator_range<v10_type, v13_type>(v10, v13)), hasher(boost::fusion::iterator_range<v10_type, v13_type>(v10, v13)));
	BOOST_CHECK_EQUAL(hasher(boost::fusion::iterator_range<v10_type, v13_type>(v10, v13)), hasher(boost::fusion::iterator_range<v20_type, v23_type>(v20, v23)));
	BOOST_CHECK_EQUAL(hasher(boost::fusion::iterator_range<v20_type, v23_type>(v20, v23)), hasher(boost::fusion::iterator_range<v20_type, v23_type>(v20, v23)));
	BOOST_CHECK_EQUAL(hasher(boost::fusion::iterator_range<v20_type, v24_type>(v20, v24)), hasher(boost::fusion::iterator_range<v20_type, v24_type>(v20, v24)));

	BOOST_WARN_NE(hasher(boost::fusion::iterator_range<v20_type, v24_type>(v20, v24)), hasher(boost::fusion::iterator_range<v10_type, v13_type>(v10, v13)));
}

// TODO: test hash_combine
