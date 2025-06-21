#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PropDataComponent.generated.h"

// 이 컴퍼넌트는 데이터를 Save할 때 사용될 예정이다.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UPropDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPropDataComponent();

	UFUNCTION()
	void SetPropID(const FName& InPropID);
	UFUNCTION()
	FName GetPropID() const { return PropID.RowName; }
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop Data", meta = (AllowPrivateAccess = true))
	FDataTableRowHandle PropID = FDataTableRowHandle();
};
