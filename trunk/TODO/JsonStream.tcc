namespace XTL
{
	template <typename _Outputter, typename _Spacer>
	void JsonStream<_Outputter, _Spacer>::Finish()
	{
		while (stack_.Top() != JSON_ROOT)
		{
			*this << json::end();
		}
	}
	
	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::plain_array)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				break;

			case PLAIN_ARRAY_BEGIN:
				Space();
				stack_.SetTop(PLAIN_ARRAY_ITEM);
				break;
						
			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;
				
			case PLAIN_ARRAY_ITEM:
				Char(',');
				Space();
				break;
					
			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;
				
			case PLAIN_OBJECT_KEY:
				Space();
				stack_.SetTop(PLAIN_OBJECT_VALUE);
				break;
				
			case MULTILINE_OBJECT_KEY:
				Space();
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case PLAIN_OBJECT_BEGIN:
			case PLAIN_OBJECT_VALUE:
			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of array received, while string key excepted.");

			default:
				RaiseError("Internal error");
		}

		Char('[');
		stack_.Push(PLAIN_ARRAY_BEGIN);

		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::plain_object)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				break;

			case PLAIN_ARRAY_BEGIN:
				Space();
				stack_.SetTop(PLAIN_ARRAY_ITEM);
				break;
						
			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;

			case PLAIN_ARRAY_ITEM:
				Char(',');
				Space();
				break;
					
			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;
				
			case PLAIN_OBJECT_KEY:
				Space();
				stack_.SetTop(PLAIN_OBJECT_VALUE);
				break;
				
			case MULTILINE_OBJECT_KEY:
				Space();
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case PLAIN_OBJECT_BEGIN:
			case PLAIN_OBJECT_VALUE:
			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of object received, while string key excepted.");

			default:
				RaiseError("Internal error");
		}
			
		Char('{');
		stack_.Push(PLAIN_OBJECT_BEGIN);

		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::array)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				break;

			case PLAIN_ARRAY_BEGIN:
			case PLAIN_ARRAY_ITEM:
			case PLAIN_OBJECT_BEGIN:
			case PLAIN_OBJECT_KEY:
			case PLAIN_OBJECT_VALUE:
				return *this << json::plain_array();
						
			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;
				
			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;
				
			case MULTILINE_OBJECT_KEY:
				Space();
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of array received, while string key excepted.");

			default:
				RaiseError("Internal error");
		}

		Char('[');
		spacer_.IncIndent();
		stack_.Push(MULTILINE_ARRAY_BEGIN);
			
		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::object)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				break;

			case PLAIN_ARRAY_BEGIN:
			case PLAIN_ARRAY_ITEM:
			case PLAIN_OBJECT_BEGIN:
			case PLAIN_OBJECT_KEY:
			case PLAIN_OBJECT_VALUE:
				return *this << json::plain_object();
						
			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;
				
			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				break;
				
			case MULTILINE_OBJECT_KEY:
				Space();
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_BEGIN:
			case MULTILINE_OBJECT_VALUE:
				RaiseError("Begin of object received, while string key excepted.");

			default:
				RaiseError("Internal error");
		}
			
		Char('{');
		spacer_.IncIndent();
		stack_.Push(MULTILINE_OBJECT_BEGIN);

		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::operator<< (json::end)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				RaiseError("Unexpected 'end' tag");

			case PLAIN_ARRAY_BEGIN:
				Char(']');
				break;

			case MULTILINE_ARRAY_BEGIN:
				spacer_.DecIndent();
				NextLine();
				Char(']');
				break;
				
			case PLAIN_ARRAY_ITEM:
				Space();
				Char(']');
				break;

			case MULTILINE_ARRAY_ITEM:
				spacer_.DecIndent();
				NextLine();
				Char(']');
				break;

			case PLAIN_OBJECT_BEGIN:
				Char('}');
				break;
					
			case MULTILINE_OBJECT_BEGIN:
				spacer_.DecIndent();
				NextLine();
				Char('}');
				break;

			case PLAIN_OBJECT_VALUE:
				Space();
				Char('}');
				break;
					
			case MULTILINE_OBJECT_VALUE:
				spacer_.DecIndent();
				NextLine();
				Char('}');
				break;
					
			case PLAIN_OBJECT_KEY:
			case MULTILINE_OBJECT_KEY:
				RaiseError("End of object received, while string key excepted.");

			default:
				RaiseError("Internal error");
		}

		stack_.Pop();

		return *this;
	}

	template <typename _Outputter, typename _Spacer>
	template <typename _Literal>
	JsonStream<_Outputter, _Spacer> & JsonStream<_Outputter, _Spacer>::WriteLiteral(const _Literal & literal)
	{
		switch (stack_.Top())
		{
			case JSON_ROOT:
				RaiseError("Unexpected literal at root level");

			case PLAIN_ARRAY_BEGIN:
				Space();
				literal.Write(outputter_);
				stack_.SetTop(PLAIN_ARRAY_ITEM);
				break;

			case PLAIN_ARRAY_ITEM:
				Char(',');
				Space();
				literal.Write(outputter_);
				break;
				
			case PLAIN_OBJECT_BEGIN:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				Space();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(PLAIN_OBJECT_KEY);
				break;
					
			case PLAIN_OBJECT_KEY:
				Space();
				literal.Write(outputter_);
				stack_.SetTop(PLAIN_OBJECT_VALUE);
				break;
					
			case PLAIN_OBJECT_VALUE:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				Char(',');
				Space();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(PLAIN_OBJECT_KEY);
				break;

			case MULTILINE_ARRAY_BEGIN:
				NextLine();
				literal.Write(outputter_);
				stack_.SetTop(MULTILINE_ARRAY_ITEM);
				break;
				
			case MULTILINE_ARRAY_ITEM:
				Char(',');
				NextLine();
				literal.Write(outputter_);
				break;
				
			case MULTILINE_OBJECT_BEGIN:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				NextLine();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(MULTILINE_OBJECT_KEY);
				break;

			case MULTILINE_OBJECT_KEY:
				Space();
				literal.Write(outputter_);
				stack_.SetTop(MULTILINE_OBJECT_VALUE);
				break;

			case MULTILINE_OBJECT_VALUE:
				if (!_Literal::IsString)
				{
					RaiseError("Object key as string literal expected");
				}
				Char(',');
				NextLine();
				literal.Write(outputter_);
				Space();
				Char(':');
				stack_.SetTop(MULTILINE_OBJECT_KEY);
				break;
		}

		return *this;
	}
}

