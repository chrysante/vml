#include <vml/quaternion.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("quaternion multiplication", "[quaternion]") {
    vml::quaternion<int> const a = { 1, 2, 3, 4 };
    SECTION("quaternion * quaternion") {
        vml::quaternion<int> const b = { 2, 3, 4, 1 };
        vml::quaternion<int> const ref = { -20, -6, 20, 8 };
        CHECK(a * b == ref);
    }
    SECTION("quaternion * complex") {
        vml::complex<int> const b = { 2, 3 };
        vml::quaternion<int> const ref = { -4, 7, 18, -1 };
        CHECK(a * b == ref);
    }
    SECTION("complex * quaternion") {
        vml::complex<int> const b = { 2, 3 };
        vml::quaternion<int> const ref = { -4, 7, -6, 17 };
        CHECK(b * a == ref);
    }
}

TEST_CASE("Construct from vector", "[quaternion]") {
    CHECK(vml::quaternion_int(vml::double4(1, 2, 3, 4)) ==
          vml::quaternion_int(1, 2, 3, 4));
}

TEMPLATE_TEST_CASE("quaternion math function", "[quaternion]", float, double,
                   long double) {
    SECTION("exponential") {
        using namespace vml::literals;
        SECTION("exp") {
            vml::complex<TestType> const ref = {
                (TestType)1.468693939915885157138967597326604261326956736629008722797675676,
                (TestType)2.287355287178842391208171906700501808955586256668355680938658114
            };
            CHECK(vml::exp(1 + 1_i) == vml::approx(ref));
        }
        SECTION("log") {
            vml::complex<TestType> const ref = {
                (TestType)0.346573590279972654708616060729088284037750067180127627060340004,
                (TestType)0.785398163397448309615660845819875721049292349843776455243736148
            };
            CHECK(vml::log(1 + 1_i) == vml::approx(ref));
        }
        SECTION("log10") {
            vml::complex<TestType> const ref = {
                (TestType)0.150514997831990597606869447362246513384094940731054270655213730,
                (TestType)0.341094088460460336871445906357838942552531870930981349975213682
            };
            CHECK(vml::log10(1 + 1_i) == vml::approx(ref));
        }
        SECTION("sqrt") {
            vml::complex<TestType> const ref = {
                (TestType)1.09868411346780996603980119524067837854439312092715774374441157,
                (TestType)0.45508986056222734130435775782246856962019037848315009258825956
            };
            CHECK(vml::sqrt(1 + 1_i) == vml::approx(ref));
        }
    }
    SECTION("trigonometric") {
        using namespace vml::literals;
        SECTION("sin") {
            vml::quaternion<TestType> const ref = {
                (TestType)1.29845758141597729482604236580781562031343656163520807340184210,
                (TestType)0.63496391478473610825508220299150978151708195141937941052695294,
                0, 0
            };
            CHECK(vml::sin(1 + 1_i + 0_j) == vml::approx(ref));
        }
        SECTION("cos") {
            vml::quaternion<TestType> const ref = {
                (TestType)0.833730025131149048883885394335094479809874785209629312270722734,
                (TestType)-0.988897705762865096382129540892686188642149695033147607536816010,
                0, 0
            };
            CHECK(vml::cos(1 + 1_i + 0_j) == vml::approx(ref));
        }
        SECTION("tan") {
            vml::quaternion<TestType> const ref = {
                (TestType)0.27175258531951171652884372249858892070946411146177945309808462,
                (TestType)1.08392332733869454347575206121197172134496752747538985631972204,
                0, 0
            };
            CHECK(vml::tan(1 + 1_i + 0_j) == vml::approx(ref));
        }
        SECTION("asin") {
            vml::complex<TestType> const ref = {
                (TestType)0.66623943249251525510400489597779272066749013872594784283147384,
                (TestType)1.06127506190503565203301891621357348580678549893863369639721028
            };
            CHECK(vml::asin(1 + 1_i) == vml::approx(ref));
        }
        SECTION("acos") {
            vml::complex<TestType> const ref = {
                (TestType)0.90455689430238136412731679566195872143109456096160506765599845,
                (TestType)-1.06127506190503565203301891621357348580678549893863369639721028
            };
            CHECK(vml::acos(1 + 1_i) == vml::approx(ref));
        }
        SECTION("atan") {
            vml::complex<TestType> const ref = {
                (TestType)1.01722196789785136772278896155048292206356087698683658714920269,
                (TestType)0.40235947810852509365018983330654690988140033856712943047816197
            };
            CHECK(vml::atan(1 + 1_i) == vml::approx(ref));
        }
        SECTION("sinh") {
            vml::quaternion<TestType> const ref = {
                (TestType)0.63496391478473610825508220299150978151708195141937941052695294,
                (TestType)1.29845758141597729482604236580781562031343656163520807340184210,
                0, 0
            };
            CHECK(vml::sinh(1 + 1_i + 0_j) == vml::approx(ref));
        }
        SECTION("cosh") {
            vml::quaternion<TestType> const ref = {
                (TestType)0.833730025131149048883885394335094479809874785209629312270722734,
                (TestType)0.988897705762865096382129540892686188642149695033147607536816010,
                0, 0
            };
            CHECK(vml::cosh(1 + 1_i + 0_j) == vml::approx(ref));
        }
        SECTION("tanh") {
            vml::quaternion<TestType> const ref = {
                (TestType)1.08392332733869454347575206121197172134496752747538985631972204,
                (TestType)0.27175258531951171652884372249858892070946411146177945309808462,
                0, 0
            };
            CHECK(vml::tanh(1 + 1_i + 0_k) == vml::approx(ref));
        }
        SECTION("asinh") {
            vml::complex<TestType> const ref = {
                (TestType)1.06127506190503565203301891621357348580678549893863369639721028,
                (TestType)0.66623943249251525510400489597779272066749013872594784283147384
            };
            CHECK(vml::asinh(1 + 1_i) == vml::approx(ref));
        }
        SECTION("acosh") {
            vml::complex<TestType> const ref = {
                (TestType)1.06127506190503565203301891621357348580678549893863369639721028,
                (TestType)0.90455689430238136412731679566195872143109456096160506765599845
            };
            CHECK(vml::acosh(1 + 1_i) == vml::approx(ref));
        }
        SECTION("atanh") {
            vml::complex<TestType> const ref = {
                (TestType)0.40235947810852509365018983330654690988140033856712943047816197,
                (TestType)1.01722196789785136772278896155048292206356087698683658714920269
            };
            CHECK(vml::atanh(1 + 1_i) == vml::approx(ref));
        }
    }
}
