
# ProfilerTLS
멀티 스레드용 성능 측정 도구

## 사전 세팅

- profiler / common.h
	- nsProfiler::MAX_PROFILER_NUM 
		> 프로파일링 가능한 태그의 최대 수
	- nsProfiler::TAG_LENGTH
		> 태그 이름의 최대 길이
- profilerTLS / common.h
	- nsProfilerTLS::THREAD_NUM
		> 프로파일러를 사용할 최대 스레드 수
- dump
	- 덤프 생성을 위해 사용 전에 CDump 객체 생성자 호출 필요

## 함수
 - nsProfilerTLS::eErrorCode begin(const char* tag)
	 > 입력된 tag에 대하여 성능 측정 시작 <br>
	> 성공 시 0 리턴, 오류 시 에러코드 리턴
 - nsProfilerTLS::eErrorCode end(const char* tag)
	 > 입력된 tag에 대하여 성능 측정 종료
	> 성공 시 0 리턴, 오류 시 에러코드 리턴
 - void printToFile()
	 > 현재까지 수집된 성능 자료를 파일로 출력 <br>
	 > profiler.txt 에 저장됨
 - void reset()
	 > 현재까지 수집된 성능 자료를 초기화

## 에러 코드
```cpp
enum nsProfilerTLS::enErrorCode {
	SUCCESS = 0,
	TOO_LONG_TAG, // 태그 이름이 너무 김
	TOO_MANY_PROFILE, // profile 배열이 모자람
	NO_PROFILE_DATA, // profile 데이터가 없음

	TOO_MANY_THREAD = 100, // 사용하는 스레드가 너무 많음
	NO_PROFILER // profiler가 할당되지 않음
};
```

## 사용법
```cpp

CDump dump;

void main(){

  CProfilerTLS p;

  for(int i=0 ; i<10 ; i++){
    p.begin("tag")
    // 측정 대상
    p.end("tag")
  }

  p.printToFile();

}
```
