#pragma once

namespace nsProfilerTLS {

	constexpr int THREAD_NUM = 50;

	enum eErrorCode {

		SUCCESS = 0,
		TOO_LONG_TAG, // �±� �̸��� �ʹ� ��
		TOO_MANY_PROFILE, // profile �迭�� ���ڶ�
		NO_PROFILE_DATA, // profile �����Ͱ� ����

		TOO_MANY_THREAD = 100, // ����ϴ� �����尡 �ʹ� ����
		NO_PROFILER // profiler�� �Ҵ���� ����
	};

};