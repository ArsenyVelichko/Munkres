template<class Matrix, class T>
Index2D findInMatrix(const Matrix& mat, const T& value) {
	for (int i = 0; i < mat.rows(); i++) {
		for (int j = 0; j < mat.cols(); j++) {
			if (mat(i, j) == value) {
				return { i, j };
			}
		}
	}
	return {};
}