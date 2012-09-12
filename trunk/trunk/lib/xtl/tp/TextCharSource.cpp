#include "TextCharSource.hpp"

namespace XTL
{
	TextCharSource::TextCharSource(std::auto_ptr<CharSource> charSource)
		: charSource_(charSource)
	{
		;;
	}

	TextCharSource::~TextCharSource() throw()
	{
		;;
	}

	bool TextCharSource::NotAtEnd() const
	{
		return charSource_->NotAtEnd();
	}

	bool TextCharSource::AtEnd() const
	{
		return charSource_->AtEnd();
	}

	char TextCharSource::GetChar() const
	{
		return charSource_->GetChar();
	}

	unsigned int TextCharSource::GetCharIndex() const
	{
		return charSource_->GetCharIndex();
	}

	void TextCharSource::Advance()
	{
		if (charSource_->GetChar() == '\n')
		{
			textCursor_.NextRow();
		}
		else
		{
			textCursor_.NextColumn();
		}

		charSource_->Advance();
	}

	unsigned int TextCharSource::Mark()
	{
		return charSource_->Mark();
	}

	void TextCharSource::Unmark()
	{
		charSource_->Unmark();
	}

	unsigned int TextCharSource::MarkIndex() const
	{
		return charSource_->MarkIndex();
	}

	const std::string TextCharSource::ReleaseString()
	{
		return charSource_->ReleaseString();
	}

	void TextCharSource::ReleaseString(CharBuffer & buffer)
	{
		charSource_->ReleaseString(buffer);
	}

	const TextCursor TextCharSource::GetCursor() const
	{
		return textCursor_;
	}

	const TextCursor TextCharSource::ReleaseCursor()
	{
		return charSource_->ReleaseCursor();
	}

	void TextCharSource::RestorePosition()
	{
		charSource_->RestorePosition();
	}
}
