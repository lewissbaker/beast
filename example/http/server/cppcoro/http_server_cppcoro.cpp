//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, using cppcoro
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <cppcoro/async_scope.hpp>
#include <cppcoro/io_service.hpp>
#include <cppcoro/on_scope_exit.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/when_all.hpp>
#include <cppcoro/net/socket.hpp>
#include <experimental/resumable>
#include <iostream>
#include <memory>
#include <utility>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = cppcoro::net;

//------------------------------------------------------------------------------

// Handles an HTTP client connection
cppcoro::task<>
do_http_session(
    std::string_view doc_root,
    net::socket sock)
{
    http::request_parser<http::string_body> rp;
    beast::flat_buffer b;
    char buf[100];
    co_await sock.recv(buf, sizeof(buf));
}

// Accepts incoming connections and launches the sessions
cppcoro::task<>
do_listen(
    std::string_view doc_root,
    net::ipv4_endpoint ep,
    cppcoro::io_service& ios)
{
    cppcoro::async_scope scope;
    net::socket acceptor(net::socket::create_tcpv4(ios));
    acceptor.bind(ep);
    acceptor.listen();
    for(;;)
    {
        net::socket sock(net::socket::create_tcpv4(ios));
        co_await acceptor.accept(sock);
        scope.spawn(do_http_session(doc_root, std::move(sock)));
    }
};

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 5)
    {
        std::cerr <<
            "Usage: http-server-cppcoro <address> <port> <doc_root> <threads>\n" <<
            "Example:\n" <<
            "    http-server-cppcoro 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    auto const address = net::ipv4_address::from_string(argv[1]);
    if(! address)
    {
        std::cerr <<
            "Invalid IPv4 address: '" << argv[1] << "'\n";
        return EXIT_FAILURE;
    }

    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::string_view(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // The io_service is required for all I/O
    cppcoro::io_service ios;

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ios]
        {
            ios.process_events();
        });
    cppcoro::sync_wait(cppcoro::when_all(
        [&]() -> cppcoro::task<>
        {
			auto stop = cppcoro::on_scope_exit(
                [&]
                {
                    ios.stop();
                });

            // Create and launch a listening port
            co_await do_listen(
                doc_root,
                net::ipv4_endpoint(*address, port),
                ios);
        }(),
        [&]() -> cppcoro::task<>
        {
            ios.process_events();
            co_return;
        }()
    ));

    return EXIT_SUCCESS;
}
