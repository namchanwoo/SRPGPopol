#pragma once

#include "CoreMinimal.h"
#include "SRGLog.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"
#include "UObject/SoftObjectPath.h"
#include "AssetTableRef.generated.h"


#define DT_DATA_TABLE_PATH TEXT("DataTable'/Game/Game_/AssetRef/DT_DatableRef.DT_DatableRef'")
#define DT_ASSET_PATH TEXT("DataTable'/Game/Game_/AssetRef/DT_AssetRef.DT_AssetRef'")
#define DT_BLUEPRINT_PATH TEXT("DataTable'/Game/Game_/AssetRef/DT_BlueprintRef.DT_BlueprintRef'")


USTRUCT()
struct FAssetRefTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UObject> AssetRef;
};

class FDataTableUtil
{
public:
	static FString GetAssetPath(const FString& DataTablePath, const FName& RowName)
	{
		static const FString ASSERTION_FAILED = TEXT("Assertion failed");

		// 생성자에서 호출되고 있는지 확인
		FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
		if (ThreadContext.IsInConstructor <= 0)
		{
			SRPG_LOG_ERROR(TEXT("%s: Called outside of constructor"), *ASSERTION_FAILED);
			return FString();
		}

		// DataTable 로드
		UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
		if (!DataTable)
		{
			SRPG_LOG_ERROR(TEXT("%s: Failed to load DataTable '%s'"), *ASSERTION_FAILED, *DataTablePath);
			return FString();
		}

		// DataTable에서 행 찾기
		FAssetRefTableRow* Row = DataTable->FindRow<FAssetRefTableRow>(RowName, FString(), false);
		if (!Row)
		{
			SRPG_LOG_ERROR(TEXT("%s: Failed to find row '%s' in DataTable '%s'"), *ASSERTION_FAILED, *RowName.ToString(), *DataTablePath);
			return FString();
		}

		// 행에서 자산 경로 가져오기
		FString AssetPath = Row->AssetRef.ToSoftObjectPath().GetAssetPathString();
		if (!AssetPath.EndsWith(RowName.ToString()))
		{
			SRPG_LOG_ERROR(TEXT("Asset path '%s' does not match row name '%s'"), *AssetPath, *RowName.ToString());
			return FString();
		}

		// 경로 끝에서 자산 이름을 제거하십시오.
		return AssetPath.LeftChop(RowName.ToString().Len() + 1);
	}


	template <class T>
	static T* LoadAsset(const FString& AssetPath)
	{
		return LoadObject<T>(nullptr, *AssetPath);
	}

	template <class T>
	static T* FindObject(const FString& DataTablePath, const FName& RowName)
	{
		FString AssetPath = GetAssetPath(DataTablePath, RowName);
		if (AssetPath.IsEmpty())
		{
			SRPG_LOG_ERROR(TEXT("Failed to find object with row name '%s' in DataTable '%s'"), *RowName.ToString(), *DataTablePath);
			return nullptr;
		}
		T* Result = LoadAsset<T>(AssetPath);
		if (!Result)
		{
			SRPG_LOG_ERROR(TEXT("Failed to load object of type '%s' with row name '%s' from DataTable '%s'"), *T::StaticClass()->GetName(),
			               *RowName.ToString(), *DataTablePath);
		}
		return Result;
	}

	template <class T>
	static TSubclassOf<T> FindClass(const FString& DataTablePath, const FName& RowName)
	{
		FString AssetPath = GetAssetPath(DataTablePath, RowName);
		if (AssetPath.IsEmpty())
		{
			SRPG_LOG_ERROR(TEXT("Failed to find class with row name '%s' in DataTable '%s'"), *RowName.ToString(), *DataTablePath);
			return TSubclassOf<T>();
		}

		ConstructorHelpers::FClassFinder<T> ClassFinder(*AssetPath);
		UClass* LoadedClass = ClassFinder.Class;

		if (!LoadedClass)
		{
			SRPG_LOG_ERROR(TEXT("Failed to load class of type '%s' with row name '%s' from DataTable '%s'"), *T::StaticClass()->GetName(),
			               *RowName.ToString(), *DataTablePath);
		}

		return TSubclassOf<T>(LoadedClass);
	}
};

typedef FDataTableUtil DT;
