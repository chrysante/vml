#include <complex>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <vml/__complex.hpp>

TEMPLATE_TEST_CASE("complex math function", "[complex]", float, double, long double) {
	SECTION("exponential") {
		using namespace mtl::literals;
		SECTION("exp") {
			mtl::complex<TestType> const ref = {
				(TestType)1.468693939915885157138967597326604261326956736629008722797675676,
				(TestType)2.287355287178842391208171906700501808955586256668355680938658114
			};
			CHECK(mtl::exp(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("log") {
			mtl::complex<TestType> const ref = {
				(TestType)0.346573590279972654708616060729088284037750067180127627060340004,
				(TestType)0.785398163397448309615660845819875721049292349843776455243736148
			};
			CHECK(mtl::log(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("log10") {
			mtl::complex<TestType> const ref = {
				(TestType)0.150514997831990597606869447362246513384094940731054270655213730,
				(TestType)0.341094088460460336871445906357838942552531870930981349975213682
			};
			CHECK(mtl::log10(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("sqrt") {
			mtl::complex<TestType> const ref = {
				(TestType)1.09868411346780996603980119524067837854439312092715774374441157,
				(TestType)0.45508986056222734130435775782246856962019037848315009258825956
			};
			CHECK(mtl::sqrt(1 + 1_i) == mtl::approx(ref));
		}
	}
	SECTION("trigonometric") {
		using namespace mtl::literals;
		SECTION("sin") {
			mtl::complex<TestType> const ref = {
				(TestType)1.29845758141597729482604236580781562031343656163520807340184210,
				(TestType)0.63496391478473610825508220299150978151708195141937941052695294
			};
			CHECK(mtl::sin(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("cos") {
			mtl::complex<TestType> const ref = {
				(TestType) 0.833730025131149048883885394335094479809874785209629312270722734,
				(TestType)-0.988897705762865096382129540892686188642149695033147607536816010
			};
			CHECK(mtl::cos(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("tan") {
			mtl::complex<TestType> const ref = {
				(TestType)0.27175258531951171652884372249858892070946411146177945309808462,
				(TestType)1.08392332733869454347575206121197172134496752747538985631972204
			};
			CHECK(mtl::tan(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("asin") {
			mtl::complex<TestType> const ref = {
				(TestType)0.66623943249251525510400489597779272066749013872594784283147384,
				(TestType)1.06127506190503565203301891621357348580678549893863369639721028
			};
			CHECK(mtl::asin(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("acos") {
			mtl::complex<TestType> const ref = {
				(TestType) 0.90455689430238136412731679566195872143109456096160506765599845,
				(TestType)-1.06127506190503565203301891621357348580678549893863369639721028
			};
			CHECK(mtl::acos(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("atan") {
			mtl::complex<TestType> const ref = {
				(TestType)1.01722196789785136772278896155048292206356087698683658714920269,
				(TestType)0.40235947810852509365018983330654690988140033856712943047816197
			};
			CHECK(mtl::atan(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("sinh") {
			mtl::complex<TestType> const ref = {
				(TestType)0.63496391478473610825508220299150978151708195141937941052695294,
				(TestType)1.29845758141597729482604236580781562031343656163520807340184210
			};
			CHECK(mtl::sinh(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("cosh") {
			mtl::complex<TestType> const ref = {
				(TestType)0.833730025131149048883885394335094479809874785209629312270722734,
				(TestType)0.988897705762865096382129540892686188642149695033147607536816010
			};
			CHECK(mtl::cosh(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("tanh") {
			mtl::complex<TestType> const ref = {
				(TestType)1.08392332733869454347575206121197172134496752747538985631972204,
				(TestType)0.27175258531951171652884372249858892070946411146177945309808462
			};
			CHECK(mtl::tanh(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("asinh") {
			mtl::complex<TestType> const ref = {
				(TestType)1.06127506190503565203301891621357348580678549893863369639721028,
				(TestType)0.66623943249251525510400489597779272066749013872594784283147384
			};
			CHECK(mtl::asinh(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("acosh") {
			mtl::complex<TestType> const ref = {
				(TestType)1.06127506190503565203301891621357348580678549893863369639721028,
				(TestType)0.90455689430238136412731679566195872143109456096160506765599845
			};
			CHECK(mtl::acosh(1 + 1_i) == mtl::approx(ref));
		}
		SECTION("atanh") {
			mtl::complex<TestType> const ref = {
				(TestType)0.40235947810852509365018983330654690988140033856712943047816197,
				(TestType)1.01722196789785136772278896155048292206356087698683658714920269
			};
			CHECK(mtl::atanh(1 + 1_i) == mtl::approx(ref));
		}
	}
}
