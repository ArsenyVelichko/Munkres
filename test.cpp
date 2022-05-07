#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "Munkres.h"

using namespace Catch::literals;

template<class ValueType>
ValueType costValue(const Eigen::MatrixX<ValueType>& mat,
					const std::vector<Munkres::Index2D>& indices) {

	ValueType sum(0);
	for (const auto& idx : indices) {
		sum += mat(idx.rowIdx, idx.colIdx);
	}
	return sum;
}

TEST_CASE( "Solver test case") {
	Munkres::Solver<double> solver;

	SECTION("3x3 matrix") {
		Eigen::MatrixX<double> mat(3, 3);

		mat << 5.1, 9.2, 1.3,
				10.4, 3.5, 2.6,
				8.7, 7.8, 4.9;

		const auto& solution = solver.solve(mat);
		REQUIRE(costValue(mat, solution) == 13.5_a);
	}

	SECTION("5x5 matrix") {
		Eigen::MatrixX<double> mat(5, 5);

		mat << 12.01, 9.02, 27.03, 10.04, 23.05,
			   7.06, 13.07, 13.08, 30.09, 19.1,
			   25.11, 18.12, 26.13, 11.14, 26.15,
			   9.16, 28.17, 26.18, 23.19, 13.2,
			   16.21, 16.22, 24.23, 6.24, 9.25;

		const auto& solution = solver.solve(mat);
		REQUIRE(costValue(mat, solution) == 51.65_a);
	}

	SECTION("5x5 matrix, maximize") {
		Eigen::MatrixX<double> mat(5, 5);

		mat << 94.01, 66.02, 100.03, 18.04, 48.05,
			   51.06, 63.07, 97.08, 79.09, 11.1,
			   37.11, 53.12, 57.13, 78.14, 28.15,
			   59.16, 43.17, 97.18, 88.19, 48.2,
			   52.21, 19.22, 89.23, 60.24, 60.25;

		const auto& solution = solver.solve(mat, true);
		REQUIRE(costValue(mat, solution) == 392.65_a);
	}

	SECTION("5x4 matrix, rectangular") {
		Eigen::MatrixX<double> mat(5, 4);

		mat << 34.01, 26.02, 17.03, 12.04,
			   43.05, 43.06, 36.07, 10.08,
			   97.09, 47.1, 66.11, 34.12,
			   52.13, 42.14, 19.15, 36.16,
			   15.17, 93.18, 55.19, 80.2;

		const auto& solution = solver.solve(mat);
		REQUIRE(costValue(mat, solution) == 70.42_a);
	}
}