#pragma once

namespace Munkres::Utility {
		class Index2D {
		public:
			Index2D();
			Index2D(int rowIxd, int colIdx);

			bool isValid() const;

			int rowIdx() const;
			int colIdx() const;

		private:
			int m_rowIdx;
			int m_colIdx;
		};

		template<class Matrix, class T>
		Index2D findInMatrix(const Matrix& mat, const T& value);

#include "Utility.tpp"

}