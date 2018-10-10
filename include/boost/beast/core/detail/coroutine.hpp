//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//
// This is a derivative work
//

#ifndef BOOST_BEAST_DETAIL_COROUTINE_HPP
#define BOOST_BEAST_DETAIL_COROUTINE_HPP

namespace boost {
namespace beast {
namespace detail {

class coroutine_ref;

/// Provides support for implementing stackless coroutines.
class coroutine
{
public:
    /// Constructs a coroutine in its initial state.
    coroutine() = default;

    /// Returns true if the coroutine is the child of a fork.
    bool is_child() const
    {
        return value_ < 0;
    }

    /// Returns true if the coroutine is the parent of a fork.
    bool is_parent() const
    {
        return !is_child();
    }

    /// Returns true if the coroutine has reached its terminal state.
    bool is_complete() const
    {
        return value_ == -1;
    }

    /// Returns true if the coroutine has ever yielded
    bool did_yield() const
    {
        return value_ != 0; // VFALCO or >0 ? how should forks work ?
    }

private:
    friend class detail::coroutine_ref;

    int value_ = 0;
};

class coroutine_ref
{
public:
    void operator=(coroutine_ref const&) = delete;
    coroutine_ref(coroutine& c)
        : value_(c.value_)
    {
    }
  
    coroutine_ref(coroutine* c)
        : value_(c->value_)
    {
    }

    ~coroutine_ref()
    {
        if(!modified_)
            value_ = -1;
    }
    operator int() const
    {
        return value_;
    }

    int& operator=(int v)
    {
        modified_ = true;
        return value_ = v;
    }

private:
    int& value_;
    bool modified_ = false;
};

#define BOOST_BEAST_CORO_REENTER(c) \
  switch (::boost::beast::detail::coroutine_ref _coro_value = c) \
    case -1: if (_coro_value) \
    { \
      goto terminate_coroutine; \
      terminate_coroutine: \
      _coro_value = -1; \
      goto bail_out_of_coroutine; \
      bail_out_of_coroutine: \
      break; \
    } \
    else /* fall-through */ case 0:

#define BOOST_BEAST_CORO_YIELD_IMPL(n) \
  for (_coro_value = (n);;) \
    if (_coro_value == 0) \
    { \
      case (n): ; \
      break; \
    } \
    else \
      switch (_coro_value ? 0 : 1) \
        for (;;) \
          /* fall-through */ case -1: if (_coro_value) \
            goto terminate_coroutine; \
          else for (;;) \
            /* fall-through */ case 1: if (_coro_value) \
              goto bail_out_of_coroutine; \
            else /* fall-through */ case 0:

#define BOOST_BEAST_CORO_FORK_IMPL(n) \
  for (_coro_value = -(n);; _coro_value = (n)) \
    if (_coro_value == (n)) \
    { \
      case -(n): ; \
      break; \
    } \
    else

#if defined(_MSC_VER)
# define BOOST_BEAST_CORO_YIELD BOOST_BEAST_CORO_YIELD_IMPL(__COUNTER__ + 1)
# define BOOST_BEAST_CORO_FORK BOOST_BEAST_CORO_FORK_IMPL(__COUNTER__ + 1)
#else // defined(_MSC_VER)
# define BOOST_BEAST_CORO_YIELD BOOST_BEAST_CORO_YIELD_IMPL(__LINE__)
# define BOOST_BEAST_CORO_FORK BOOST_BEAST_CORO_FORK_IMPL(__LINE__)
#endif // defined(_MSC_VER)

} // detail
} // beast
} // boost

#endif
