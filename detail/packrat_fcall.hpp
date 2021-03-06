/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#define BOOST_PP_FILENAME_1 \
    "detail/packrat_fcall.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, SPIRIT_ARGUMENTS_LIMIT)
#include BOOST_PP_ITERATE()

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <BOOST_PP_ENUM_PARAMS(N, typename A)>
    typename boost::lazy_enable_if_c<
        (params_size == N)
      , boost::proto::terminal<
            boost::spirit::qi::parameterized_nonterminal<
                parameterized_subject_type
              , boost::fusion::vector<BOOST_PP_ENUM_PARAMS(N, A)> >
        >
    >::type
    operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& f)) const
    {
        typedef boost::fusion::vector<BOOST_PP_ENUM_PARAMS(N, A)> vector_type;
        typedef boost::spirit::qi::parameterized_nonterminal<
            parameterized_subject_type, vector_type> parameterized_type;
        typedef typename boost::proto::terminal<parameterized_type>::type result_type;

        return result_type::make(
            parameterized_type(
                this->get_parameterized_subject()
              , boost::fusion::make_vector(BOOST_PP_ENUM_PARAMS(N, f)))
        );
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


