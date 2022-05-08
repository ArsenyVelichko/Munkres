#pragma once

#include <Eigen/Dense>

namespace Munkres {

	struct Index2D {
	public:
		Index2D() : rowIdx(-1), colIdx(-1) {}
		Index2D(int rowIxd, int colIdx) : rowIdx(rowIxd), colIdx(colIdx) {}

		bool isValid() const {
			return colIdx >= 0 && rowIdx >= 0;
		}

		int rowIdx;
		int colIdx;
	};

	template<class ValueType>
	class Solver {
	public:
		Solver();

		using CostMatrix = Eigen::MatrixX<ValueType>;

		std::vector<Index2D> solve(const CostMatrix& costMatrix, bool maximize = false);

	private:
		int minDimension() const;
		bool isNonCovered(int rowIdx, int colIdx) const;

		void preliminaries(const CostMatrix& costMatrix, bool maximize);
		std::vector<Index2D> result() const;

		ValueType findMinNonCovered() const;

		Index2D findNonCoveredZero() const;

		Index2D findStarredInRow(int rowIdx) const;
		Index2D findPrimedInRow(int rowIdx) const;
		Index2D findStarredInCol(int colIdx) const;


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

		std::vector<Index2D> m_primedPath;
		std::vector<Index2D> m_starredPath;
	};

#include "Munkres.tpp"

}