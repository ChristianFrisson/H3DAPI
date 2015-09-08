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

#ifndef MINIDUMP_H
#define MINIDUMP_H

#if defined( WIN32 )
#include <Windows.h>
#include <DbgHelp.h>

#include <string>

#if defined( _MSC_VER )
	#pragma comment( lib, "Dbghelp.lib" )
#endif

class MiniDump {
public:
	typedef enum InfoLevel {
		kInfoLevelSmall,
		kInfoLevelMedium,
		kInfoLevelLarge
	} InfoLevel;

	MiniDump();
	virtual ~MiniDump();

	bool Create( const wchar_t *inPath,
				InfoLevel inLevel = kInfoLevelMedium,
				PEXCEPTION_POINTERS pExceptionPtrs = NULL,
				bool inSuspendOtherThreads = false ) const;

private:
	std::wstring mProcessNameWithoutExtension;

	static BOOL CALLBACK MinidumpFilter( PVOID inParam, const
							PMINIDUMP_CALLBACK_INPUT inInput,
							PMINIDUMP_CALLBACK_OUTPUT outOutput );

	bool IsDataSegmentNeeded( const wchar_t *inPath ) const;
	void EnumerateThreads( DWORD (WINAPI *fpCallback)( HANDLE ), DWORD inExceptThisOne ) const;

	static DWORD CALLBACK MinidumpWriter( LPVOID inParam );
};
#endif
#endif // MINIDUMP_H