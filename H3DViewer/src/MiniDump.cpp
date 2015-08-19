// Copyright (c) 2011 Aaron Ballman (aaron@aaronballman.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "MiniDump.h"

#if defined( WIN32 )

#include <TlHelp32.h>


bool MiniDump::IsDataSegmentNeeded( const wchar_t *inPath ) const
{
	if (!inPath)	return false;

	// Get the name of the module we're trying to query.  We
	// only want to allow the data segments for the bare minimum
	// number of modules.  That includes the kernel, and the process
	// itself.  However, we may want to extend this in the future
	// to allow the application to register modules it cares about too.
	wchar_t fileName[ MAX_PATH ] = { 0 };
	::_wsplitpath_s( inPath, NULL, 0, NULL, 0, fileName, MAX_PATH, NULL, 0 );

	if (::_wcsicmp( fileName, mProcessNameWithoutExtension.c_str() ) == 0 ||
		::_wcsicmp( fileName, L"kernel32" ) == 0 ||
		::_wcsicmp( fileName, L"ntdll" ) == 0) {
		return true;
	}
	return false;
}

// We use this structure as a way to pass some information to our static
// callback.  It's only used for transport.
struct MinidumpCallbackParam {
	MiniDump::InfoLevel infoLevel;
	const MiniDump *dump;
	DWORD thread;
};

BOOL CALLBACK MiniDump::MinidumpFilter( PVOID inParam, const PMINIDUMP_CALLBACK_INPUT inInput, PMINIDUMP_CALLBACK_OUTPUT outOutput )
{
	if (!inInput || !outOutput || !inParam)	return FALSE;

	// Most of the time, we're going to let the minidump writer do whatever
	// it needs to do.  However, when trying to limit information for things
	// like smaller dumps, we want to filter out various things.  So this callback
	// is used as a way to filter out information that the user has said they
	// don't want.
	//
	// If we were so inclined, we could use a delegate class to allow the user
	// to customize the dump files even further.  But for right now, this is
	// close enough.
	struct MinidumpCallbackParam *param = (struct MinidumpCallbackParam *)inParam;
	InfoLevel infoLevel = param->infoLevel;

	switch (inInput->CallbackType) {
		case IncludeModuleCallback:
		case ThreadCallback:
		case ThreadExCallback:		return TRUE;
		case CancelCallback:		return FALSE;

		case IncludeThreadCallback: {
			// We don't want to include information about the minidump writing
			// thread, as that's not of interest to the caller
			if (inInput->IncludeThread.ThreadId == param->thread)	return FALSE;
			return TRUE;
		} break;

		case MemoryCallback: {
			// Small and medium sized dumps don't need full memory access
			if (kInfoLevelSmall == infoLevel ||
				kInfoLevelMedium == infoLevel)	return FALSE;
			return TRUE;
		} break;

		case ModuleCallback: {
			if (kInfoLevelSmall == infoLevel) {
				// When creating a small dump file, we filter out any modules that
				// aren't being directly referenced.
				if (!(outOutput->ModuleWriteFlags & ModuleReferencedByMemory)) {
					outOutput->ModuleWriteFlags &= ~ModuleWriteModule;
				}
			} else if (kInfoLevelMedium == infoLevel) {
				// When creating a medium-sized dump file, we filter out any module
				// data segments if they're not part of our core module list.  This
				// helps reduce the size of the dump file by quite a bit.
				if (outOutput->ModuleWriteFlags & ModuleWriteDataSeg) {
					if (!param->dump->IsDataSegmentNeeded( inInput->Module.FullPath )) {
						outOutput->ModuleWriteFlags &= ~ModuleWriteDataSeg;
					}
				}
			}

			return TRUE;
		} break;
	}

	return FALSE;
}

struct MiniDumpParams {
	const wchar_t *path;
	MiniDump::InfoLevel level;
	const MiniDump *dump;
};

DWORD CALLBACK MiniDump::MinidumpWriter( LPVOID inParam )
{
	struct MiniDumpParams *p = (MiniDumpParams *)inParam;

	// First, attempt to create the file that the minidump will be written to
	HANDLE hFile = ::CreateFileW( p->path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (INVALID_HANDLE_VALUE != hFile) {
		// Determine what information we want the minidumper to pass along for our
		// callback to filter out.  Generally speaking, the more things we include,
		// the larger our file will be.
		int type = MiniDumpNormal;
		switch (p->level) {
			case kInfoLevelSmall: {
				type |= MiniDumpWithIndirectlyReferencedMemory |
						MiniDumpScanMemory;
			} break;
			case kInfoLevelMedium: {
				type |= MiniDumpWithDataSegs |
						MiniDumpWithPrivateReadWriteMemory |
						MiniDumpWithHandleData |
						MiniDumpWithFullMemoryInfo |
						MiniDumpWithThreadInfo |
						MiniDumpWithUnloadedModules;
			} break;
			case kInfoLevelLarge: {
				type |= MiniDumpWithDataSegs |
						MiniDumpWithPrivateReadWriteMemory |
						MiniDumpWithHandleData |
						MiniDumpWithFullMemory |
						MiniDumpWithFullMemoryInfo |
						MiniDumpWithThreadInfo |
						MiniDumpWithUnloadedModules |
						MiniDumpWithProcessThreadData;
			} break;
		}

		// Set up the callback to be called by the minidump writer.  This allows us to
		// filter out information that we may not care about.
		MINIDUMP_CALLBACK_INFORMATION callback = { 0 };
		struct MinidumpCallbackParam info = { p->level, p->dump, ::GetCurrentThreadId() };
		callback.CallbackParam = &info;
		callback.CallbackRoutine = MinidumpFilter;

		// After all that, we can write out the minidump
		BOOL bRet = ::MiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), hFile,
				(MINIDUMP_TYPE)type, NULL, NULL, &callback );
		::CloseHandle( hFile );

		return bRet ? 1 : 0;
	}

	return 0;
}

void MiniDump::EnumerateThreads( DWORD (WINAPI *inCallback)( HANDLE ), DWORD inExceptThisOne ) const
{
	// Create a snapshot of all the threads in the process, and walk over
	// them, calling the callback function on each of them, except for
	// the thread identified by the inExceptThisOne parameter.
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
	if (INVALID_HANDLE_VALUE != hSnapshot) {
		THREADENTRY32 thread;
		thread.dwSize = sizeof( thread );
		if (::Thread32First( hSnapshot, &thread )) {
			do {
				if (thread.th32OwnerProcessID == ::GetCurrentProcessId() &&
					thread.th32ThreadID != inExceptThisOne &&
					thread.th32ThreadID != ::GetCurrentThreadId()) {
					// We're making a big assumption that this call will only
					// be used to suspend or resume a thread, and so we know
					// that we only require the THREAD_SUSPEND_RESUME access right.
					HANDLE hThread = ::OpenThread( THREAD_SUSPEND_RESUME, FALSE, thread.th32ThreadID );
					if (hThread) {
						inCallback( hThread );
						::CloseHandle( hThread );
					}
				}
			} while (::Thread32Next( hSnapshot, &thread ) );
		}

		::CloseHandle( hSnapshot );
	}
}

MiniDump::MiniDump()
{
	// We need to keep track of the name of the process, sans extension, so that it can
	// be used if we're filtering module data segments.  So we calculate that up front.
	wchar_t processName[ MAX_PATH ] = { 0 };
	::GetModuleFileNameW( NULL, processName, MAX_PATH );
	::_wsplitpath_s( processName, NULL, 0, NULL, 0, processName, MAX_PATH, NULL, 0 );
	mProcessNameWithoutExtension = processName;
}

MiniDump::~MiniDump()
{
}

bool MiniDump::Create( const wchar_t *inPath, InfoLevel inLevel, bool inSuspendOtherThreads ) const {
	if (!inPath)	return false;

	// Set up the list of parameters we want to pass to the newly-created thread.  Note
	// that we create the thread in a suspended state!
	struct MiniDumpParams param = { inPath, inLevel, this };
	DWORD threadId = 0;
	HANDLE hThread = ::CreateThread( NULL, 0, MinidumpWriter, &param, CREATE_SUSPENDED, &threadId );

	if (hThread) {
		// Having created the thread successfully, we need to put all of the other
		// threads in the process into a suspended state, making sure not to suspend
		// our newly-created thread.  We do this because we want this function to
		// behave as a snapshot, and that means other threads should not continue
		// to perform work while we're creating the minidump.
		if (inSuspendOtherThreads) {
			EnumerateThreads( ::SuspendThread, threadId );
		}

		// Now we can resume our worker thread
		::ResumeThread( hThread );

		// Wait for the thread to finish working, without allowing the current
		// thread to continue working.  This ensures that the current thread won't
		// do anything interesting while we're writing the debug information out.
		// This also means that the minidump will show this as the current callstack.
		::WaitForSingleObject( hThread, INFINITE );

		// The thread exit code tells us whether we were able to create the minidump
		DWORD code = 0;
		::GetExitCodeThread( hThread, &code );
		::CloseHandle( hThread );

		// If we suspended other threads, now is the time to wake them up
		if (inSuspendOtherThreads) {
			EnumerateThreads( ::ResumeThread, threadId );
		}

		return code != 0;
	}
	return false;
}

#endif
