/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_SEQUENCE_JAN_06_2013_1015AM)
#define SPIRIT_SEQUENCE_JAN_06_2013_1015AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/operator/detail/sequence.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename Left, typename Right>
    struct sequence : binary_parser<Left, Right, sequence<Left, Right>>
    {
        typedef binary_parser<Left, Right, sequence<Left, Right>> base_type;

        sequence(Left left, Right right)
            : base_type(left, right) {}

        template <typename Iterator, typename Context>
        bool parse(
            Iterator& first, Iterator const& last
          , Context& context, unused_type) const
        {
            Iterator save = first;
            if (this->left.parse(first, last, context, unused)
                && this->right.parse(first, last, context, unused))
                return true;
            first = save;
            return false;
        }

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(
            Iterator& first, Iterator const& last
          , Context& context, Attribute& attr) const
        {
            typedef detail::partition_attribute<Left, Right, Attribute> partition;
            typedef typename partition::l_pass l_pass;
            typedef typename partition::r_pass r_pass;

            typename partition::l_range left_seq = partition::left(attr);
            typename partition::r_range right_seq = partition::right(attr);
            typename l_pass::type l_attr = l_pass::call(left_seq);
            typename r_pass::type r_attr = r_pass::call(right_seq);

            Iterator save = first;
            if (this->left.parse(first, last, context, l_attr)
                && this->right.parse(first, last, context, r_attr))
                return true;
            first = save;
            return false;
        }
    };

    template <typename Left, typename Right>
    inline sequence<
        typename extension::as_parser<Left>::value_type
      , typename extension::as_parser<Right>::value_type>
    operator>>(Left const& left, Right const& right)
    {
        typedef sequence<
            typename extension::as_parser<Left>::value_type
          , typename extension::as_parser<Right>::value_type>
        result_type;

        return result_type(as_parser(left), as_parser(right));
    }
}}}

namespace boost { namespace spirit { namespace traits
{
    template <typename Left, typename Right>
    struct attribute_of<x3::sequence<Left, Right>>
        : x3::detail::attribute_of_sequence<Left, Right> {};
}}}

#endif