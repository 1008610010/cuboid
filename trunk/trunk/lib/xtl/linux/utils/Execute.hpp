#ifndef XTL__EXECUTE_HPP__
#define XTL__EXECUTE_HPP__ 1

#include <sys/types.h>

#include <string>

#include "../UnixError.hpp"

namespace XTL
{
	class ChildExit
	{
	};

	class ForkExecListener
	{
		public:

			virtual ~ForkExecListener() throw() { ;; }

			virtual void OnExecError(const std::string & filePath, const UnixError & e) const = 0;

			virtual void OnDoubleForkError(const UnixError & e) const { ;; }
	};

	void WaitPid(pid_t pid);

	void Exec(const std::string & filePath, const ForkExecListener & listener);

	/**
		Выполнить заданную программу и подождать ее завершения.
		listener.OnExecError(...) вызывается в дочернем процессе при ошибке вызова execve().
		@throw XTL::UnixError  выбрасывается в родительском процессе при ошибке в системном вызове.
	*/
	void ForkExecWait(const std::string & filePath, const ForkExecListener & listener);

	/**
	 * @throw XTL::UnixError выбрасывает родительский процесс в случае ошибки fork() или waitpid().
	 * @throw XTL::ChildExit выбрасывает дочерний процесс.
	 */
	void DoubleForkExec(const std::string & filePath, const ForkExecListener & listener);
}

#endif
