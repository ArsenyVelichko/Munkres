#include <limits>
#include <algorithm>

#include "Munkres.h"

template<class ValueType>
Solver<ValueType>::Solver() : m_colNumber(0), m_rowNumber(0), m_primedPath(1) {}

template<class ValueType>
void Solver<ValueType>::preliminaries(const Solver::CostMatrix& costMatrix) {
	m_result = costMatrix;

	m_colNumber = m_result.cols();
	m_rowNumber = m_result.rows();

	m_coveredRows.resize(m_rowNumber);
	m_coveredCols.resize(m_colNumber);
	uncoverLines();

	m_markerMask = MarkerMask::Zero(m_rowNumber, m_colNumber);

	size_t k = minDimension();
	m_primedPath.reserve(k + 1);
	m_starredPath.reserve(k);
}

template<class ValueType>
void Solver<ValueType>::solve(const Solver::CostMatrix& costMatrix) {
	preliminaries(costMatrix);

	int nextStep = 0;
	const auto& steps = algoSteps();

	while (nextStep >= 0) {
		nextStep = (this->*steps[nextStep])();
	}
}

template<class ValueType>
int Solver<ValueType>::step0() {
	if (m_rowNumber > m_colNumber) {
		m_result.rowwise() -= m_result.colwise().minCoeff();
	} else {
		m_result.colwise() -= m_result.rowwise().minCoeff();
	}
	return 1;
}

template<class ValueType>
int Solver<ValueType>::step1() {
	for (int i = 0; i < m_rowNumber; i++) {
		for (int j = 0; j < m_colNumber; j++) {
			if (m_result(i, j) == 0 && isNonCovered(i, j)) {
				m_markerMask(i, j) = StarredZero;
				m_coveredRows[i] = true;
				m_coveredCols[j] = true;
			}
		}
	}

	uncoverLines();
	return 2;
}

template<class ValueType>
int Solver<ValueType>::step2() {
	const auto& starredMask = (m_markerMask == StarredZero);
	m_coveredCols = starredMask.colwise().any();

	if (m_coveredCols.count() == minDimension()) { return -1; }
	return 3;
}

template<class ValueType>
int Solver<ValueType>::step3() {
	while (true) {
		auto idx = findNonCoveredZero();
		if (!idx.isValid()) { return 5; }

		m_markerMask(idx.rowIdx(), idx.colIdx()) = PrimedZero;
		m_primedPath[0] = idx;

		idx = findStarredInRow(idx.rowIdx());
		if (!idx.isValid()) { return 4; }

		m_coveredCols(idx.colIdx()) = false;
		m_coveredRows(idx.rowIdx()) = true;
	}
}

template<class ValueType>
int Solver<ValueType>::step4() {
	Utility::Index2D primedIdx = m_primedPath[0];

	for (int i = 0; /*i < minDimension()*/; i++) {
		const auto& starredIdx = findStarredInCol(primedIdx.colIdx());
		if (!starredIdx.isValid()) { break; }

		m_starredPath.push_back(starredIdx);
		primedIdx = findPrimedInRow(starredIdx.rowIdx());
		m_primedPath.push_back(primedIdx);
	}

	postprocessPaths();
	uncoverLines();
	erasePrimes();

	return 2;
}

template<class ValueType>
int Solver<ValueType>::step5() {
	ValueType h = findMinNonCovered();

	for (int i = 0; i < m_rowNumber; i++) {
		for (int j = 0; j < m_colNumber; j++) {
			if (m_coveredRows[i]) {
				m_result(i, j) += h;
			}

			if (!m_coveredCols[j]) {
				m_result(i, j) -= h;
			}
		}
	}
	return 3;
}

template<class ValueType>
Utility::Index2D Solver<ValueType>::findNonCoveredZero() const {
	for (int i = 0; i < m_rowNumber; i++) {
		for (int j = 0; j < m_colNumber; j++) {
			if (m_result(i, j) == 0 && isNonCovered(i, j)) {
				return { i, j };
			}
		}
	}
	return {};
}

template<class ValueType>
ValueType Solver<ValueType>::findMinNonCovered() const {
	ValueType minValue = std::numeric_limits<ValueType>::max();

	for (int i = 0; i < m_rowNumber; i++) {
		for (int j = 0; j < m_colNumber; j++) {
			if (minValue > m_result(i, j) && isNonCovered(i, j)) {
				minValue = m_result(i, j);
			}
		}
	}
	return minValue;
}

template<class ValueType>
Utility::Index2D Solver<ValueType>::findStarredInRow(int rowIdx) const {
	const auto& row = m_markerMask.row(rowIdx);
	const auto& localIdx = Utility::findInMatrix(row, StarredZero);
	return { rowIdx, localIdx.colIdx() };
}

template<class ValueType>
Utility::Index2D Solver<ValueType>::findStarredInCol(int colIdx) const {
	const auto& col = m_markerMask.col(colIdx);
	const auto& localIdx = Utility::findInMatrix(col, StarredZero);
	return { localIdx.rowIdx(), colIdx };
}

template<class ValueType>
Utility::Index2D Solver<ValueType>::findPrimedInRow(int rowIdx) const {
	const auto& row = m_markerMask.row(rowIdx);
	const auto& localIdx = Utility::findInMatrix(row, PrimedZero);
	return { rowIdx, localIdx.colIdx() };
}

template<class ValueType>
std::vector<typename Solver<ValueType>::Step> Solver<ValueType>::algoSteps() {
	return { &Solver::step0, &Solver::step1,
			 &Solver::step2, &Solver::step3,
			 &Solver::step4, &Solver::step5 };
}

template<class ValueType>
int Solver<ValueType>::minDimension() const {
	return std::min(m_colNumber, m_rowNumber);
}

template<class ValueType>
void Solver<ValueType>::postprocessPaths() {
	for (const auto& idx : m_primedPath) {
		m_markerMask(idx.rowIdx(), idx.colIdx()) = StarredZero;
	}

	for (const auto& idx : m_starredPath) {
		m_markerMask(idx.rowIdx(), idx.colIdx()) = NoMarker;
	}

	m_primedPath.resize(1);
	m_starredPath.clear();
}

template<class ValueType>
void Munkres::Solver<ValueType>::uncoverLines() {
	std::fill(m_coveredRows.begin(), m_coveredRows.end(), false);
	std::fill(m_coveredCols.begin(), m_coveredCols.end(), false);
}

template<class ValueType>
void Munkres::Solver<ValueType>::erasePrimes() {
	auto ravel = m_markerMask.reshaped();
	std::replace(ravel.begin(), ravel.end(), PrimedZero, NoMarker);
}

template<class ValueType>
bool Solver<ValueType>::isNonCovered(int rowIdx, int colIdx) const {
	return !m_coveredRows[rowIdx] && !m_coveredCols[colIdx];
}

template<class ValueType>
typename Solver<ValueType>::Index2DArray Solver<ValueType>::result() const {
	Index2DArray starredIndices;

	for (int i = 0; i < m_rowNumber; i++) {
		for (int j = 0; j < m_colNumber; j++) {
			if (m_markerMask(i, j) == StarredZero) {
				starredIndices.emplace_back(i, j);
			}
		}
	}
	return starredIndices;
}



