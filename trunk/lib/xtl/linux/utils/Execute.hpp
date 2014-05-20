#ifndef XTL__EXECUTE_HPP__
#define XTL__EXECUTE_HPP__ 1

#include <sys/types.h>

#include <string>
#include <vector>

#include "../UnixError.hpp"

namespace XTL
{
	class ChildExit
	{
	};

	class ForkExecErrorListener
	{
		public:

			virtual ~ForkExecErrorListener() throw() { ;; }

			virtual void OnExecError(const std::string & filePath, const UnixError & e) const = 0;

			virtual void OnDoubleForkError(const UnixError & e) const { ;; }
	};

	void WaitPid(pid_t pid);

	void Exec(const std::string & filePath, const ForkExecErrorListener & errorListener);

	void Exec(const std::string & filePath, const std::vector<std::string> & arguments, const ForkExecErrorListener & errorListener);

	/**
		Выполнить заданную программу и подождать ее завершения.
		errorListener.OnExecError(...) вызывается в дочернем процессе при ошибке вызова execvpe().
		@throw XTL::UnixError  выбрасывается в родительском процессе при ошибке в системном вызове fork.
	*/
	void ForkExecWait(const std::string           & filePath,
	                  const ForkExecErrorListener & errorListener);

	/**
		Выполнить заданную программу с переданными аргументами и подождать ее завершения.
		errorListener.OnExecError(...) вызывается в дочернем процессе при ошибке вызова execvpe().
		@throw XTL::UnixError  выбрасывается в родительском процессе при ошибке в системном вызове fork.
	*/
	void ForkExecWait(const std::string              & filePath,
	                  const std::vector<std::string> & arguments,
	                  const ForkExecErrorListener    & errorListener);

	void ForkExecWait(const std::string           & filePath,
	                  const std::string           & arg1,
	                  const ForkExecErrorListener & errorListener);

	void ForkExecWait(const std::string           & filePath,
	                  const std::string           & arg1,
	                  const std::string           & arg2,
	                  const ForkExecErrorListener & errorListener);

	/**
	 * @throw XTL::UnixError выбрасывает родительский процесс в случае ошибки fork() или waitpid().
	 * @throw XTL::ChildExit выбрасывает дочерний процесс.
	 */
	void DoubleForkExec(const std::string              & filePath,
	                    const std::vector<std::string> & arguments,
	                    const ForkExecErrorListener    & errorListener);

	void DoubleForkExec(const std::string              & filePath,
	                    const ForkExecErrorListener    & errorListener);

	void DoubleForkExec(const std::string              & filePath,
	                    const std::string              & arg1,
	                    const ForkExecErrorListener    & errorListener);
}

#endif
