//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/uri/make_shared.hpp>

#include <boost/beast/unit_test/suite.hpp>

namespace boost {
namespace beast {
namespace uri {

class make_shared_test : public unit_test::suite
{
public:
    void
    run() override
    {
        view v;
        auto sv = make_shared(v);
        pass();
    }
};

BEAST_DEFINE_TESTSUITE(beast,uri,make_shared);

} // uri
} // beast
} // boost
