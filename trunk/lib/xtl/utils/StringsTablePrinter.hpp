#ifndef XTL__STRINGS_TABLE_PRINTER_HPP__
#define XTL__STRINGS_TABLE_PRINTER_HPP__ 1

namespace XTL
{
	class CellAlignment
	{
		public:

			static const CellAlignment LEFT;
			static const CellAlignment CENTER;
			static const CellAlignment RIGHT;

			/**
			 * По умолчанию CellAlignment::LEFT
			 */
			CellAlignment();

			bool operator== (const CellAlignment & other) const;

		private:

			explicit CellAlignment(unsigned int id);

			unsigned int id_;
	};

}

#endif
