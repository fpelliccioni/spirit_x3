/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_LEXEME_MARCH_24_2007_0802AM)
#define SPIRIT_LEXEME_MARCH_24_2007_0802AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/context.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename Subject>
    struct lexeme_directive : unary_parser<Subject, lexeme_directive<Subject>>
    {
        typedef unary_parser<Subject, lexeme_directive<Subject> > base_type;
        static bool const is_pass_through_unary = true;

        lexeme_directive(Subject const& subject)
          : base_type(subject) {}

        typedef typename
            traits::attribute_of<Subject>::type
        attribute_type;

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            x3::skip_over(first, last, context);
            auto skipper = get<skipper_tag>(context);

            typedef unused_skipper<
                typename remove_reference<decltype(skipper)>::type>
            unused_skipper_type;
            unused_skipper_type unused_skipper(skipper);

            return this->subject.parse(
                first, last
              , make_context<skipper_tag>(unused_skipper, context)
              , attr);
        }
    };

    struct lexeme_gen
    {
        template <typename Subject>
        lexeme_directive<typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            typedef
                lexeme_directive<typename extension::as_parser<Subject>::value_type>
            result_type;

            return result_type(as_parser(subject));
        }
    };

    lexeme_gen const lexeme = lexeme_gen();
}}}



#endif
