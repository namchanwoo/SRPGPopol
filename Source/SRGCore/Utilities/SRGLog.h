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


#define SRPG_CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}
#define SRPG_IF_CHECK(Expr, ErrorMsg, ...) {if(!(Expr)) {SRPG_LOG_ERROR(ErrorMsg); return __VA_ARGS__;}}


#define SRPG_LOG_TIME(Verbosity, Format, ...) UE_LOG(SRPGLOG, Verbosity, TEXT("%s %s %s"), *FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S")), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define SRPG_NOT_IMPLEMENTED() SRPG_LOG_ERROR(TEXT("%s 함수는 아직 구현되지 않았습니다."), __FUNCTION__)
