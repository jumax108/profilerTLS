#pragma once

#include "dump/headers/dump.h"
#ifdef _DEBUG
	#pragma comment(lib, "lib/dump/MSVC/debug/dump")
#else
	#pragma comment(lib, "lib/dump/MSVC/release/dump")
#endif


#include "profiler/headers/profiler.h"
#ifdef _DEBUG
	#pragma comment(lib, "lib/profiler/MSVC/debug/profiler")
#else
	#pragma comment(lib, "lib/profiler/MSVC/release/profiler")
#endif

#include "common.h"

class CProfilerTLS {


public:

	CProfilerTLS();

	nsProfilerTLS::eErrorCode begin(const char* tag);
	nsProfilerTLS::eErrorCode end(const char* tag);

	void printToFile();

	void reset();

private:
	struct stProfilerSingleThread {
		CProfiler _profiler;
		int _threadID;

		stProfilerSingleThread() {
			_threadID = -1;
		}
	};

private:

	int _tlsIndex;

	int _profilerAllocIndex;
	stProfilerSingleThread _profiler[nsProfilerTLS::THREAD_NUM];

};