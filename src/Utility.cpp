#include "Utility.h"

using namespace Munkres::Utility;

Index2D::Index2D() : m_rowIdx(-1), m_colIdx(-1) {}

Index2D::Index2D(int rowIxd, int colIdx) : m_rowIdx(rowIxd), m_colIdx(colIdx) {}

bool Index2D::isValid() const {
	return m_colIdx != -1 && m_rowIdx != -1;
}

int Index2D::rowIdx() const {
	return m_rowIdx;
}

int Index2D::colIdx() const {
	return m_colIdx;
}
