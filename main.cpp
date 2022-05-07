#include <iostream>
#include <Eigen/Dense>

#include "Munkres.h"

int main() {
	Munkres::Solver<int> solver;

	int n, m;
	std::cout << "Matrix size:" << std::endl;
	std::cin >> n >> m;

	Eigen::MatrixXi matA(n, m);

	for (int i = 0; i < matA.rows(); i++) {
		for (int j = 0; j < matA.cols(); j++) {
			matA(i, j) = (i + 1) * (j + 1);
		}
	}

	std::cout <<  matA << std::endl;
	auto indices = solver.solve(matA);

	double sum = 0.0;
	for (const auto& idx : indices) {
		sum += matA(idx.rowIdx, idx.colIdx);
	}
	std::cout << "Sum: " << sum << std::endl;

	return 0;
}
