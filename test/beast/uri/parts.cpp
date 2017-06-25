//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/uri/parts.hpp>

#include <boost/beast/unit_test/suite.hpp>

namespace boost {
namespace beast {
namespace uri {

class parts_test : public unit_test::suite
{
public:
    void
    run() override
    {
        log << "sizeof(parts)==" << sizeof(parts) << std::endl;
        pass();
    }
};

BEAST_DEFINE_TESTSUITE(beast,uri,parts);

} // uri
} // beast
} // boost

