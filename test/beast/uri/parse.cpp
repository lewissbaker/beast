//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/uri/parse.hpp>

#include <boost/beast/uri/make_shared.hpp>
#include <boost/beast/unit_test/suite.hpp>

namespace boost {
namespace beast {
namespace uri {

class parse_test : public unit_test::suite
{
public:
    void
    match(
        string_view u,
        string_view scheme,
        string_view authority = "",
        string_view path = "",
        string_view query = "",
        string_view fragment = "")
    {
        error_code ec;
        static_buffer<256> out;
        parse_absolute_form(out, u, ec);
        log << "scheme    = '" << out.scheme() << "'" << std::endl;
        log << "authority = '" << out.authority() << "'" << std::endl;
        log << "path      = '" << out.path() << "'" << std::endl;
        log << "query     = '" << out.query() << "'" << std::endl;

        BEAST_EXPECTS(! ec, ec.message());
        BEAST_EXPECT(out.scheme() == scheme);
        BEAST_EXPECT(out.authority() == authority);
        BEAST_EXPECT(out.path() == path);
        BEAST_EXPECT(out.query() == query);
    }

    void
    testAbsolute()
    {
        match("http://a.b.c",           "http", "a.b.c", "/");
        match("http://a.b.c/",          "http", "a.b.c", "/");
        match("http://a.b.c/a",         "http", "a.b.c", "/a");
        match("http://a.b.c/ab",        "http", "a.b.c", "/ab");
        match("http://a.b.c/a/b",       "http", "a.b.c", "/a/b");
        match("http://a.b.c/a/b?",      "http", "a.b.c", "/a/b");
        match("http://a.b.c/a/b?x",     "http", "a.b.c", "/a/b", "x");
        match("http://a.b.c/a/b?x=",    "http", "a.b.c", "/a/b", "x=");
        match("http://a.b.c/a/b?x=1",   "http", "a.b.c", "/a/b", "x=1");
    }

    void
    run() override
    {
        testAbsolute();
    }
};

BEAST_DEFINE_TESTSUITE(beast,uri,parse);

} // uri
} // beast
} // boost

