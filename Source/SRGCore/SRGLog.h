#pragma once

#include "CoreMinimal.h"


SRGCORE_API DECLARE_LOG_CATEGORY_EXTERN(SRPGLOG, Log, All); //추가

#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG_S(Verbosity) UE_LOG(SRPGLOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)

#define LOG(Verbosity, Format, ...) UE_LOG(SRPGLOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define SRPG_LOG(Verbosity, Format, ...) UE_LOG(SRPGLOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define SRPG_LOG_WARNING(Format, ...) UE_LOG(SRPGLOG, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define SRPG_LOG_ERROR(Format, ...) UE_LOG(SRPGLOG, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	SRPG_LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__))
#define	SRPG_LOG_SCREEN_T(Time, Format, ...) GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

#define SRPG_LOG_SCREEN_ERROR(Format, ...) \
{ \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__)); \
SRPG_LOG_ERROR(Format, ##__VA_ARGS__); \
}


// assert()와 거의 비슷한 용도로 사용가능한 매크로. ...에는 retval이 들어감.
#define SRPG_CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

// 조건부 로깅 매크로 추가
#define SRPG_LOG_IF(Condition, Verbosity, Format, ...) if (Condition) { SRPG_LOG(Verbosity, Format, ##__VA_ARGS__); }

// 시간 로깅 매크로 추가
#define SRPG_LOG_TIME(Verbosity, Format, ...) UE_LOG(SRPGLOG, Verbosity, TEXT("%s %s %s"), *FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S")), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


class FFunctionLogger
{
public:
	FFunctionLogger(const TCHAR* FunctionName)
		: Function(FunctionName)
	{
		SRPG_LOG(Log, TEXT("Entering function: %s"), Function);
	}

	~FFunctionLogger()
	{
		SRPG_LOG(Log, TEXT("Leaving function: %s"), Function);
	}

private:
	const TCHAR* Function;
};

// 함수 시작 및 종료 로깅: 함수 시작과 종료를 자동으로 로깅하여, 함수의 실행 흐름을 쉽게 추적할 수 있습니다. 이를 위해 RAII 클래스를 사용하여 함수 시작과 종료를 추적합니다.
#define SRPG_LOG_FUNCTION FFunctionLogger FunctionLogger(*FString(__FUNCTION__))
// #define SRPG_LOG_FUNCTION FFunctionLogger FunctionLogger(TEXT(__FUNCTION__))





class FScopedTimer
{
public:
	FScopedTimer(const TCHAR* InScopeName)
		: ScopeName(InScopeName)
		  , StartTime(FPlatformTime::Seconds())
	{
	}

	~FScopedTimer()
	{
		double EndTime = FPlatformTime::Seconds();
		double TimeElapsed = EndTime - StartTime;
		SRPG_LOG(Log, TEXT("'%s' took %f seconds"), ScopeName, TimeElapsed);
	}

private:
	const TCHAR* ScopeName;
	double StartTime;
};

// 범위 기반 타이머 로깅: 코드 블록의 실행 시간을 측정하고 로깅하는 기능입니다. 이를 위해 RAII 클래스를 사용하여 범위 시작과 종료를 추적합니다.
#define SRPG_LOG_SCOPED_TIMER(ScopeName) FScopedTimer ScopedTimer(TEXT(ScopeName))

