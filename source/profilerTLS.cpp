#include "../headers/profilerTLS.h"

CProfilerTLS::CProfilerTLS() {

	_tlsIndex = TlsAlloc();
	if (_tlsIndex == TLS_OUT_OF_INDEXES) {
		CDump::crash();
	}

	_profilerAllocIndex = 0;

}

nsProfilerTLS::eErrorCode CProfilerTLS::begin(const char* tag) {

	stProfilerSingleThread* profiler = 
		(stProfilerSingleThread*)TlsGetValue(_tlsIndex);

	if (profiler == nullptr) {
		int profilerAllocIndex = InterlockedIncrement((LONG*)&_profilerAllocIndex) - 1;

		if (profilerAllocIndex >= nsProfilerTLS::THREAD_NUM) {
			InterlockedDecrement((LONG*)&_profilerAllocIndex);
			return nsProfilerTLS::eErrorCode::TOO_MANY_THREAD;
		}

		profiler = &_profiler[profilerAllocIndex];
		TlsSetValue(_tlsIndex, profiler);
	}

	return (nsProfilerTLS::eErrorCode)profiler->_profiler.begin(tag);

}

nsProfilerTLS::eErrorCode CProfilerTLS::end(const char* tag) {

	stProfilerSingleThread* profiler = 
		(stProfilerSingleThread*)TlsGetValue(_tlsIndex);

	if (profiler == nullptr) {
		return nsProfilerTLS::eErrorCode::NO_PROFILER;
	}

	return (nsProfilerTLS::eErrorCode)profiler->_profiler.end(tag);

}

void CProfilerTLS::printToFile() {

	stProfilerSingleThread* profilerIter = _profiler;
	stProfilerSingleThread* profilerEnd = _profilerAllocIndex + _profiler;
	CProfiler::stProfile totalProfile[nsProfiler::MAX_PROFILE_NUM];

	FILE* outFile;
	fopen_s(&outFile, "profilerTLS.txt", "w");
	if (outFile == nullptr) {
		CDump::crash();
	}

	fprintf_s(outFile, "(100ns)\n");
	fprintf_s(outFile, "%15s | %20s | %15s | %15s | %15s | %15s \n", "Thread ID", "Name", "Average", "Min", "Max", "Call");
	
	for (; profilerIter != profilerEnd; ++profilerIter) {

		CProfiler::stProfile profileData = profilerIter->_profiler.begin();

		for (; profileData._tag[0] != '\0'; profileData = profilerIter->_profiler.next()) {


			if (profileData._callCnt <= 2) {
				continue;
			}

			profileData._sum = 
				profileData._sum - profileData._max - profileData._min;
			profileData._callCnt -= 2;

			fprintf_s(outFile, "%15d | %20s | %12.3lf us | %12lld us | %12lld us | %15d \n",
				profilerIter->_threadID,
				profileData._tag,
				(double)profileData._sum / profileData._callCnt,
				profileData._min,
				profileData._max,
				profileData._callCnt);

			for (int profileCnt = 0; profileCnt < nsProfiler::MAX_PROFILE_NUM; ++profileCnt) {

				if (strcmp(profileData._tag, totalProfile[profileCnt]._tag) == 0) {

					totalProfile[profileCnt]._sum += profileData._sum;
					totalProfile[profileCnt]._callCnt += profileData._callCnt;
					break;
				}

				else if (totalProfile[profileCnt]._tag[0] == '\0') {

					strcpy_s(totalProfile[profileCnt]._tag, profileData._tag);
					totalProfile[profileCnt]._sum += profileData._sum;
					totalProfile[profileCnt]._callCnt += profileData._callCnt;
					break;
				}
			}

			

		}



	}

	fprintf_s(outFile, "%20s | %15s | %15s \n", "Name", "Average", "Call");

	for (int profileCnt = 0; profileCnt < nsProfiler::MAX_PROFILE_NUM; ++profileCnt) {
		if (totalProfile[profileCnt]._callCnt <= 0) {
			continue;
		}

		fprintf_s(outFile, "%20s | %12.3lf us | %15d \n",
			totalProfile[profileCnt]._tag,
			(double)totalProfile[profileCnt]._sum / (double)totalProfile[profileCnt]._callCnt,
			totalProfile[profileCnt]._callCnt);
	}
}

void CProfilerTLS::reset() {

	for (int profilerCnt = 0; profilerCnt < nsProfilerTLS::THREAD_NUM; ++profilerCnt) {
		_profiler[profilerCnt]._profiler.reset();
	}

}