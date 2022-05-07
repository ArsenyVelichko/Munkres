#include "Index2D.h"

using namespace Munkres;

Index2D::Index2D() : rowIdx(-1), colIdx(-1) {}

Index2D::Index2D(int rowIxd, int colIdx) : rowIdx(rowIxd), colIdx(colIdx) {}

bool Index2D::isValid() const {
	return colIdx >= 0 && rowIdx >= 0;
}
