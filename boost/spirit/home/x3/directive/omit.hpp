/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_OMIT_MARCH_24_2007_0802AM)
#define SPIRIT_OMIT_MARCH_24_2007_0802AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    // omit_directive forces the attribute of subject parser
    // to be unused_type
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject>
    struct omit_directive : unary_parser<Subject, omit_directive<Subject>>
    {
        typedef unary_parser<Subject, omit_directive<Subject> > base_type;
        typedef unused_type attribute_type;
        static bool const has_attribute = false;

        typedef Subject subject_type;
        omit_directive(Subject const& subject)
          : base_type(subject) {}

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return this->subject.parse(first, last, context, attr);
        }
    };

    struct omit_gen
    {
        template <typename Subject>
        omit_directive<typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            typedef
                omit_directive<typename extension::as_parser<Subject>::value_type>
            result_type;

            return result_type(as_parser(subject));
        }
    };

    omit_gen const omit = omit_gen();
}}}

#endif
