#pragma once

namespace Munkres {
		struct Index2D {
		public:
			Index2D();
			Index2D(int rowIxd, int colIdx);

			bool isValid() const;

			int rowIdx;
			int colIdx;
		};
}