#ifndef _XC2__MESSAGE_QUEUE_HPP__
#define _XC2__MESSAGE_QUEUE_HPP__ 1

#include <sup/Types.h>

namespace XC2
{
	template <typename T>
	struct MessageWrapper
	{
		long mtype;
		T body;
	};

	class MessageQueue
	{
		public:

			enum Result
			{
				OK      = 0,
				ERROR   = 1,
				WAIT    = 2,
				CLOSED  = 3,
				REMOVED = 4
			};

			MessageQueue();

			bool Open(const CHAR_8 * path, CHAR_8 sym);

			bool Create(const CHAR_8 * path, CHAR_8 sym);

			void Close();

			static bool Destroy(const CHAR_8 * path, CHAR_8 sym);

			Result SendMessage(const void * msg, INT_32 size, bool wait = true);

			Result RecvMessage(long type, void * msg, INT_32 size, bool wait = true);

			/*
			WARNING: first field of _MessageType structure must be 'long mtype'!
			*/

			template <typename _MessageType>
			Result SendMessage(const _MessageType & msg, bool wait = true)
			{
				return SendMessage(&msg, sizeof(_MessageType), wait);
			}

			template <typename _MessageType>
			Result RecvMessage(long type, _MessageType & msg, bool wait = true)
			{
				return RecvMessage(type, &msg, sizeof(_MessageType), wait);
			}

		private:

			key_t key_;
			int id_;
	};
}

#endif
