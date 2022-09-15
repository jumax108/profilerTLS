#pragma once

namespace nsProfilerTLS {

	constexpr int THREAD_NUM = 50;

	enum eErrorCode {

		SUCCESS = 0,
		TOO_LONG_TAG, // 태그 이름이 너무 김
		TOO_MANY_PROFILE, // profile 배열이 모자람
		NO_PROFILE_DATA, // profile 데이터가 없음

		TOO_MANY_THREAD = 100, // 사용하는 스레드가 너무 많음
		NO_PROFILER // profiler가 할당되지 않음
	};

};