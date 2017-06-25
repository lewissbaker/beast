//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/uri/scheme.hpp>

#include <boost/beast/unit_test/suite.hpp>

namespace boost {
namespace beast {
namespace uri {

class scheme_test : public unit_test::suite
{
public:
    void
    run() override
    {
        BEAST_EXPECT(is_special(string_to_scheme("ftp")));
        BEAST_EXPECT(is_special(string_to_scheme("file")));
        BEAST_EXPECT(is_special(string_to_scheme("gopher")));
        BEAST_EXPECT(is_special(string_to_scheme("http")));
        BEAST_EXPECT(is_special(string_to_scheme("https")));
        BEAST_EXPECT(is_special(string_to_scheme("ws")));
        BEAST_EXPECT(is_special(string_to_scheme("wss")));

        BEAST_EXPECT(! is_special(string_to_scheme("magnet")));
        BEAST_EXPECT(! is_special(string_to_scheme("mailto")));
    }
};

BEAST_DEFINE_TESTSUITE(beast,uri,scheme);

} // uri
} // beast
} // boost
