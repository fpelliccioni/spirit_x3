/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/comparison.hpp>

#include <string>
#include <iostream>
#include "test.hpp"

int
main()
{
    using boost::spirit::x3::char_;
    using boost::spirit::x3::space;
    using boost::spirit::x3::string;
    //~ using boost::spirit::x3::alpha;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::unused;
    //~ using boost::spirit::x3::no_case;
    using boost::spirit::x3::int_;
    //~ using boost::spirit::x3::double_;
    //~ using boost::spirit::x3::what;
    using boost::spirit::x3::rule;
    //~ using boost::spirit::x3::_1;
    //~ using boost::spirit::x3::_2;
    using boost::spirit::x3::alnum;

    using boost::fusion::vector;
    using boost::fusion::at_c;

    using spirit_test::test;
    using spirit_test::test_attr;

    {
        BOOST_TEST((test("aa", char_ >> char_)));
        BOOST_TEST((test("aa", char_ >> 'a')));
        BOOST_TEST((test("aaa", char_ >> char_ >> char_('a'))));
        BOOST_TEST((test("xi", char_('x') >> char_('i'))));
        BOOST_TEST((!test("xi", char_('x') >> char_('o'))));
        BOOST_TEST((test("xin", char_('x') >> char_('i') >> char_('n'))));
    }

#ifdef BOOST_SPIRIT_COMPILE_ERROR_CHECK
    {
        // Compile check only
        struct x {};
        char_ >> x(); // this should give a reasonable error message
    }
#endif

    {
        BOOST_TEST((test(" a a", char_ >> char_, space)));
        BOOST_TEST((test(" x i", char_('x') >> char_('i'), space)));
        BOOST_TEST((!test(" x i", char_('x') >> char_('o'), space)));
    }


    {
        BOOST_TEST((test(" Hello, World", lit("Hello") >> ',' >> "World", space)));
    }


    {
        vector<char, char> attr;
        BOOST_TEST((test_attr("ab", char_ >> char_, attr)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'b'));
    }

#ifdef BOOST_SPIRIT_COMPILE_ERROR_CHECK
    {
        // Compile check only
        vector<char, char> attr;

        // error: attr does not have enough elements
        test_attr("abc", char_ >> char_ >> char_, attr);
    }
#endif

    {
        vector<char, char, char> attr;
        BOOST_TEST((test_attr(" a\n  b\n  c", char_ >> char_ >> char_, attr, space)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'b'));
        BOOST_TEST((at_c<2>(attr) == 'c'));
    }

    {
        // 'b' has an unused_type. unused attributes are not part of the sequence
        vector<char, char> attr;
        BOOST_TEST((test_attr("abc", char_ >> 'b' >> char_, attr)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'c'));
    }

    {
        // 'b' has an unused_type. unused attributes are not part of the sequence
        vector<char, char> attr;
        BOOST_TEST((test_attr("acb", char_ >> char_ >> 'b', attr)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'c'));
    }

    {
        // "hello" has an unused_type. unused attributes are not part of the sequence
        vector<char, char> attr;
        BOOST_TEST((test_attr("a hello c", char_ >> "hello" >> char_, attr, space)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'c'));
    }

    {
        // a single element
        char attr;
        BOOST_TEST((test_attr("ab", char_ >> 'b', attr)));
        BOOST_TEST((attr == 'a'));
    }

    {
        // a single element fusion sequence
        vector<char> attr;
        BOOST_TEST((test_attr("ab", char_ >> 'b', attr)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
    }

    {
        // make sure single element tuples get passed through if the rhs
        // has a single element tuple as its attribute

        typedef vector<char, int> attr_type;
        attr_type fv;

        auto r = rule<class r, attr_type>()
            = char_ >> ',' >> int_;

        BOOST_TEST((test_attr("test:x,1", "test:" >> r, fv) &&
            fv == attr_type('x', 1)));
    }

    {
        // unused means we don't care about the attribute
        BOOST_TEST((test_attr("abc", char_ >> 'b' >> char_, unused)));
    }

    // $$$ no_case not yet implememnted $$$
    //~ {
        //~ BOOST_TEST((test("aA", no_case[char_('a') >> 'a'])));
        //~ BOOST_TEST((test("BEGIN END", no_case[lit("begin") >> "end"], space)));
        //~ BOOST_TEST((!test("BEGIN END", no_case[lit("begin") >> "nend"], space)));
    //~ }

    {
#ifdef SPIRIT_NO_COMPILE_CHECK
        char_ >> char_ = char_ >> char_; // disallow this!
#endif
    }

    { // alternative forms of attributes. Allow sequences to take in
      // stl containers.

        std::vector<char> v;
        BOOST_TEST(test_attr("abc", char_ >> char_ >> char_, v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == 'a');
        BOOST_TEST(v[1] == 'b');
        BOOST_TEST(v[2] == 'c');
    }

    { // alternative forms of attributes. Allow sequences to take in
      // stl containers.

        std::vector<char> v;
        BOOST_TEST(test_attr("a,b,c", char_ >> *(',' >> char_), v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == 'a');
        BOOST_TEST(v[1] == 'b');
        BOOST_TEST(v[2] == 'c');
    }

    { // alternative forms of attributes. Allow sequences to take in
      // stl containers.

        std::vector<char> v;
        BOOST_TEST(test_attr("abc", char_ >> *char_, v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == 'a');
        BOOST_TEST(v[1] == 'b');
        BOOST_TEST(v[2] == 'c');
    }

    { // alternative forms of attributes. Allow sequences to take in
      // stl containers.
        //~ using boost::spirit::qi::hold;

        std::vector<char> v;
        BOOST_TEST(test_attr("abc", char_ >> *(char_ >> char_), v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == 'a');
        BOOST_TEST(v[1] == 'b');
        BOOST_TEST(v[2] == 'c');

        v.clear();
        BOOST_TEST(!test_attr("abcd", char_ >> *(char_ >> char_), v));

        // $$$ hold not yet implementd $$$
        //~ v.clear();
        //~ BOOST_TEST(test_attr("abcdef", char_ >> *hold[char_ >> char_] >> char_, v));
        //~ BOOST_TEST(v.size() == 6);
        //~ BOOST_TEST(v[0] == 'a');
        //~ BOOST_TEST(v[1] == 'b');
        //~ BOOST_TEST(v[2] == 'c');
        //~ BOOST_TEST(v[3] == 'd');
        //~ BOOST_TEST(v[4] == 'e');
        //~ BOOST_TEST(v[5] == 'f');

        v.clear();
        BOOST_TEST(test_attr("abc", char_ >> +(char_ >> char_), v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == 'a');
        BOOST_TEST(v[1] == 'b');
        BOOST_TEST(v[2] == 'c');
    }

    { // alternative forms of attributes. Allow sequences to take in
      // stl containers.

        std::vector<char> v;
        BOOST_TEST(test_attr("abc", char_ >> -(+char_), v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == 'a');
        BOOST_TEST(v[1] == 'b');
        BOOST_TEST(v[2] == 'c');
    }

    { // alternative forms of attributes. Allow sequences to take in
      // stl containers.

        std::string s;
        BOOST_TEST(test_attr("foobar", string("foo") >> string("bar"), s));
        BOOST_TEST(s == "foobar");

        s.clear();

        // $$$ hold not yet implemented $$$
        //~ using boost::spirit::qi::hold;

        //~ rule<char const*, std::string()> word = +char_("abc");
        //~ BOOST_TEST(test_attr("ab.bc.ca", *hold[word >> string(".")] >> word, s));
        //~ BOOST_TEST(s == "ab.bc.ca");
    }

    // alternative forms of attributes. Allow sequences to take in
    // stl containers of stl containers.
    {
        std::vector<std::string> v;
        BOOST_TEST(test_attr("abc1,abc2",
            *~char_(',') >> *(',' >> *~char_(',')), v));
        BOOST_TEST(v.size() == 2 && v[0] == "abc1" && v[1] == "abc2");
    }

    {
        std::vector<std::string> v;

        auto e = rule<class e, std::string>()
            = *~char_(',');

        auto l = rule<class l, std::vector<std::string>>()
            = e >> *(',' >> e);

        BOOST_TEST(test_attr("abc1,abc2,abc3", l, v));
        BOOST_TEST(v.size() == 3);
        BOOST_TEST(v[0] == "abc1");
        BOOST_TEST(v[1] == "abc2");
        BOOST_TEST(v[2] == "abc3");
    }

    // do the same with a plain string object
    {
        std::string s;
        BOOST_TEST(test_attr("abc1,abc2",
            *~char_(',') >> *(',' >> *~char_(',')), s));
        BOOST_TEST(s == "abc1abc2");
    }

    {
        std::string s;
        auto e = rule<class e, std::string>()
            = *~char_(',');

        auto l = rule<class l, std::string>()
            = e >> *(',' >> e);

        BOOST_TEST(test_attr("abc1,abc2,abc3", l, s));
        BOOST_TEST(s == "abc1abc2abc3");
    }

    {
        std::vector<char> v;
        BOOST_TEST(test_attr("ab", char_ >> -char_, v));
        BOOST_TEST(v.size() == 2 && v[0] == 'a' && v[1] == 'b');

        v.clear();
        BOOST_TEST(test_attr("a", char_ >> -char_, v));
        BOOST_TEST(v.size() == 1 && v[0] == 'a');

        // $$$ should this be allowed? I don't think so... $$$
        //~ v.clear();
        //~ BOOST_TEST(test_attr("a", char_, v));
        //~ BOOST_TEST(v.size() == 1 && v[0] == 'a');
    }

    {
        std::vector<boost::optional<char> > v;
        BOOST_TEST(test_attr("ab", char_ >> -char_, v));
        BOOST_TEST(v.size() == 2 && v[0] == 'a' && v[1] == 'b');

        v.clear();
        BOOST_TEST(test_attr("a", char_ >> -char_, v));
        BOOST_TEST(v.size() == 2 && v[0] == 'a' && !v[1]);

        // $$$ should this be allowed? I don't think so... $$$
        //~ v.clear();
        //~ BOOST_TEST(test_attr("a", char_, v));
        //~ BOOST_TEST(v.size() == 1 && v[0] == 'a');
    }

    // test from spirit mailing list
    // "Optional operator causes string attribute concatenation"
    {
        typedef vector<char, char, int> attr_type;
        attr_type attr;

        auto node = alnum >> -('[' >> alnum >> '=' >> int_ >> ']');

        BOOST_TEST(test_attr("x[y=123]", node, attr));
        BOOST_TEST(attr == attr_type('x', 'y', 123));
    }

    // test from spirit mailing list (variation of above)
    // "Optional operator causes string attribute concatenation"
    {
        typedef vector<std::string, std::string, int> attr_type;
        attr_type attr;

        auto node = +alnum >> -('[' >> +alnum >> '=' >> int_ >> ']');

        BOOST_TEST(test_attr("xxx[yyy=123]", node, attr));
        BOOST_TEST(attr == attr_type("xxx", "yyy", 123));
    }

    // test from spirit mailing list
    // "Error with container within sequence"
    {
        typedef vector<std::string> attr_type;
        attr_type attr;

        auto r = *alnum;

        BOOST_TEST(test_attr("abcdef", r, attr));
        BOOST_TEST(at_c<0>(attr) == "abcdef");
    }

    // test from spirit mailing list (variation of above)
    // "Error with container within sequence"
    {
        typedef vector<std::vector<int>> attr_type;
        attr_type attr;

        auto r = *int_;

        BOOST_TEST(test_attr("123 456", r, attr, space));
        BOOST_TEST(at_c<0>(attr).size() == 2);
        BOOST_TEST(at_c<0>(attr)[0] == 123);
        BOOST_TEST(at_c<0>(attr)[1] == 456);
    }

    // $$$ Not yet implemented $$$
    //~ {   // test action
        //~ using boost::phoenix::ref;
        //~ char c = 0;
        //~ int n = 0;

        //~ BOOST_TEST(test("x123\"a string\"", (char_ >> int_ >> "\"a string\"")
            //~ [ref(c) = _1, ref(n) = _2]));
        //~ BOOST_TEST(c == 'x');
        //~ BOOST_TEST(n == 123);
    //~ }

    // $$$ Not yet implemented $$$
    //~ {   // test action
        //~ using boost::phoenix::ref;
        //~ char c = 0;
        //~ int n = 0;

        //~ BOOST_TEST(test("x 123 \"a string\"", (char_ >> int_ >> "\"a string\"")
            //~ [ref(c) = _1, ref(n) = _2], space));
        //~ BOOST_TEST(c == 'x');
        //~ BOOST_TEST(n == 123);
    //~ }

//     { // compile check only
//         using boost::spirit::qi::rule;
//         typedef boost::fusion::vector<int, double> tuple_type;
//         typedef std::vector<boost::fusion::vector<int, double> > attr_type;
//
//         rule<char const*, tuple_type()> r = int_ >> ',' >> double_;
//         rule<char const*, attr_type()> r2 = r >> *(',' >> r);
//         //~ rule<char const*, attr_type()> r2 = r % ',';
//     }

    return boost::report_errors();
}

