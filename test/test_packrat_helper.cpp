/***********************************************************************/
/*                                                                     */
/* test_packrat_helper.cpp: Test code helper for Packrat parser        */
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

#include "test_packrat_helper.hpp"

std::stringstream yak::spirit::debug::test_ss;

bool yak::spirit::debug::trace::enable = false;

bool yak::spirit::debug::count::enable = false;
int yak::spirit::debug::count::hit_success_ = 0;
int yak::spirit::debug::count::hit_fail_ = 0;
int yak::spirit::debug::count::parse_success_ = 0;
int yak::spirit::debug::count::parse_fail_ = 0;
