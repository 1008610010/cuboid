	class HexadecimalClassifier : protected DecimalClassifier
	{
		public:

			HexadecimalClassifier()
				: DecimalClassifier()
			{
				Set('A', 10); Set('a', 10);
				Set('B', 11); Set('b', 11);
				Set('C', 12); Set('c', 12);
				Set('D', 13); Set('d', 13);
				Set('E', 14); Set('e', 14);
				Set('F', 15); Set('f', 15);
			}
			
			const int operator() (char ch) const
			{
				return Get(ch);
			}
	};

