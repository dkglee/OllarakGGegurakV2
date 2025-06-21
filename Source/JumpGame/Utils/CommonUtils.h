#pragma once

#include "FastLogger.h"

#define GETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE type Get##varName() const { return varName; }

#define SETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE void Set##varName(type val) { varName = val; }

#define GETTER_SETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE type Get##varName() const { return varName; } \
UFUNCTION(BlueprintCallable) \
FORCEINLINE void Set##varName(type val) { varName = val; }

class FCommonUtil
{
public:
	template <typename EnumType>
	static FString GetClassEnumKeyAsString(const EnumType EnumeratorValue)
	{
		TArray<FString> EnumKey;
		UEnum::GetValueAsString(EnumeratorValue).
			ParseIntoArray(EnumKey, TEXT("::"));

		if (EnumKey.Num() == 1)
		{
			return EnumKey[0];
		}
		return EnumKey[1];
	}
	template<typename TEnum>
	static FText GetEnumDisplayName(TEnum EnumValue)
	{
		static_assert(TIsEnum<TEnum>::Value, "TEnum must be an enum type.");

		const UEnum* EnumPtr = StaticEnum<TEnum>();
		if (!EnumPtr)
		{
			return FText::FromString(TEXT("Invalid"));
		}

		return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(EnumValue));
	}
};
