/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef PROCESS_H
#define PROCESS_H

namespace icinga
{

/**
 * The result of a Process task.
 *
 * @ingroup base
 */
struct ProcessResult
{
	double ExecutionStart;
	double ExecutionEnd;
	long ExitStatus;
	String Output;
};

/**
 * A process task. Executes an external application and returns the exit
 * code and console output.
 *
 * @ingroup base
 */
class I2_BASE_API Process : public AsyncTask<Process, ProcessResult>
{
public:
	typedef shared_ptr<Process> Ptr;
	typedef weak_ptr<Process> WeakPtr;

	static const deque<Process::Ptr>::size_type MaxTasksPerThread = 512;

	Process(const String& command, const Dictionary::Ptr& environment = Dictionary::Ptr());

private:
	static bool m_ThreadCreated;

	String m_Command;
	Dictionary::Ptr m_Environment;

#ifndef _WIN32
	pid_t m_Pid;
#endif /* _WIN32 */
	FILE *m_FP;

	stringstream m_OutputStream;

	ProcessResult m_Result;

	virtual void Run(void);

	static boost::mutex m_Mutex;
	static deque<Process::Ptr> m_Tasks;
	static condition_variable m_TasksCV;

	static void WorkerThreadProc(void);

	void InitTask(void);
	bool RunTask(void);

	int GetFD(void) const;

	void Spawn(const String& command, const Dictionary::Ptr& extraEnv);
	void Spawn(const vector<String>& args, const Dictionary::Ptr& extraEnv);
	int WaitPid(void);
};

}

#endif /* PROCESS_H */
