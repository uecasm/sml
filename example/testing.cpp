//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include "boost/sml.hpp"
#include "boost/sml/testing/state_machine.hpp"

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};

struct data {
  int value = 0;
};

struct testing {
  auto operator()() const noexcept {
    using namespace sml;

    const auto guard = [](data& d) { return !d.value; };
    const auto action = [](data& d) { d.value = 42; };

    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> = "s2"_s
      , "s2"_s + event<e3> [guard] / action = X // transition under test
    );
    // clang-format on
  }
};

int main() {
  using namespace sml;
  data fake_data{0};
  testing::sm<testing> sm{fake_data};
  sm.set_current_states("s2"_s);
  sm.process_event(e3{});
  assert(sm.is(X));
  assert(fake_data.value = 42);
}
