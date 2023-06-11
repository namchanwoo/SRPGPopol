#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SRPGUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SRGCORE_API USRPGUtilityLibrary : public UObject
{
	GENERATED_BODY()

public:
	template <typename TEnum>
	static FString EnumToString(const TEnum value)
	{
		const UEnum* enumPtr = StaticEnum<TEnum>();
		if (!enumPtr)
		{
			return FString("Enum 값이 유효하지 않습니다. Invalid");
		}
		return enumPtr->GetNameByValue((int64)value).ToString();
	}


	/**
 	* @brief 주어진 배열에서 임의의 인덱스를 반환합니다.
 	* @param Array 임의 인덱스를 가져올 배열입니다.
 	* @return 임의 인덱스 또는 배열이 비어 있으면 -1입니다.
 	*/
	static int32 GetRandomIndex(const TArray<int32>& Array)
	{
		if (Array.Num() == 0)
		{
			return -1; // Return -1 if the array is empty
		}

		return FMath::RandRange(0, Array.Num() - 1);
	}

	/**
 	* @brief 배열에서 임의의 요소를 가져옵니다.
 	* @tparam T 배열의 요소 유형입니다.
 	* @param Array 무작위 요소를 가져올 배열입니다.
 	* @return 배열의 임의 요소 또는 배열이 비어 있는 경우 기본값입니다.
 	*/
	template <typename T>
	static T GetRandomElement(const TArray<T>& Array)
	{
		if (Array.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1);
			return Array[RandomIndex];
		}

		return T(); // Return a default value if the array is empty
	}

	/**
	 * @brief 배열에서 임의의 포인터 요소를 가져옵니다.
	 * @tparam T 배열에 있는 포인터 요소의 유형입니다.
	 * @param Array 무작위 요소를 가져올 배열입니다.
	 * @return 배열의 임의 포인터 요소 또는 배열이 비어 있는 경우 nullptr입니다.
	 */
	template <typename T>
	static T* GetRandomElement(const TArray<T*>& Array)
	{
		if (Array.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1);
			return Array[RandomIndex];
		}

		return nullptr; // Return nullptr if the array is empty
	}

	template <typename T>
	static void ShuffleArray(TArray<T>& ArrayToShuffle)
	{
		for (int32 i = ArrayToShuffle.Num() - 1; i > 0; --i)
		{
			int32 swapIndex = FMath::RandRange(0, i);
			if (i != swapIndex)
			{
				ArrayToShuffle.Swap(swapIndex, i);
			}
		}
	}


	/**
	 * @brief 배열의 요소 순서를 반대로 바꿉니다.
	 * @tparam T 배열의 요소 유형입니다.
	 * @param Array 반전할 배열입니다.
	 */
	template <typename T>
	static void ReverseArray(TArray<T>& Array)
	{
		for (int32 Start = 0, End = Array.Num() - 1; Start < End; Start++, End--)
		{
			Array.Swap(Start, End);
		}
	}

	/**
	 * @brief 주어진 숫자가 소수인지 확인합니다.
	 * @param Number 확인할 번호입니다.
	 * @return 숫자가 소수이면 True, 그렇지 않으면 False입니다.
	 */
	UFUNCTION(BlueprintPure, Category = "Utility")
	static bool IsPrime(int32 Number)
	{
		if (Number <= 1)
		{
			return false;
		}

		for (int32 Divisor = 2; Divisor * Divisor <= Number; Divisor++)
		{
			if (Number % Divisor == 0)
			{
				return false;
			}
		}

		return true;
	}


	// 필요에 따라 더 많은 유틸리티 기능을 추가하십시오...
};
