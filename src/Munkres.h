#pragma once

#include <Eigen/Dense>

#include "Utility.h"

namespace Munkres {
	template<class ValueType>
	class Solver {
	public:
		Solver();

		using CostMatrix = Eigen::MatrixX<ValueType>;
		using Index2DArray = std::vector<Utility::Index2D>;

		void solve(const CostMatrix& costMatrix, bool maximize = false);
		Index2DArray result() const;

	private:
		int minDimension() const;
		bool isNonCovered(int rowIdx, int colIdx) const;

		void preliminaries(const CostMatrix& costMatrix, bool maximize);

		ValueType findMinNonCovered() const;

		Utility::Index2D findNonCoveredZero() const;
		Utility::Index2D findStarredInRow(int rowIdx) const;
		Utility::Index2D findStarredInCol(int colIdx) const;
		Utility::Index2D findPrimedInRow(int rowIdx) const;

		using Step = int (Solver::*)();
		static std::vector<Step> algoSteps();

		int step0();
		int step1();
		int step2();
		int step3();
		int step4();
		int step5();

		void postprocessPaths();
		void uncoverLines();
		void erasePrimes();

		enum Marker {
			NoMarker = 0,
			StarredZero,
			PrimedZero,
		};

		using CoverLine = Eigen::VectorX<bool>;
		using MarkerMask = Eigen::ArrayXX<Marker>;

		CoverLine m_coveredRows;
		CoverLine m_coveredCols;

		MarkerMask m_markerMask;
		CostMatrix m_result;

		int m_colNumber;
		int m_rowNumber;

		Index2DArray m_primedPath;
		Index2DArray m_starredPath;
	};

#include "Munkres.tpp"

}