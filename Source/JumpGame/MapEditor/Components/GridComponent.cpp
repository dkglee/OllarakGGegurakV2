#include "GridComponent.h"

#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "JumpGame/Utils/FastLogger.h"

UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGridComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UpdatedActor = GetOwner();
}

void UGridComponent::SetSize(FVector NewSize)
{
	Size.X = FMath::RoundToInt(NewSize.X);
	Size.Y = FMath::RoundToInt(NewSize.Y);
	Size.Z = FMath::RoundToInt(NewSize.Z);
	
	// Offset = Size * SnapSize * 0.5f;
}

bool UGridComponent::MoveByGizmoPrimary(FVector MouseLocation, const FHitResult& HitResult)
{
	bool flag = true;

	// TODO: 트러블 슈팅 작성하기 (왜 음수가 필요 없어졌는가)
	// 이유 : 이전에는 Offset을 더하는 형식이라 중심 좌표를 옮겼지만 지금은 그리드의 교차점이 중점이라 Offset이 필요 없어짐
	// 그에 따라서 추가적인 음수와 관련된 연산이 필요 없어짐
	FVector Sign = FVector(1.f, 1.f, 1.f);

	Sign.X = ((int32)MouseLocation.X == 0 || (MouseLocation.X >= 0.f)) ? 1.f : -1.f;
	Sign.Y = ((int32)MouseLocation.Y == 0 || (MouseLocation.Y >= 0.f)) ? 1.f : -1.f;
	Sign.Z = ((int32)MouseLocation.Z == 0 || (MouseLocation.Z >= 0.f)) ? 1.f : -1.f;

	// GridComponent의 위치 계산
	// 그리드의 스냅 단위는 100으로 유지
	X = (int32)FMath::RoundToInt(MouseLocation.X) / ((int32)SnapSize);
	Y = (int32)FMath::RoundToInt(MouseLocation.Y) / ((int32)SnapSize);
	Z = (int32)FMath::RoundToInt(MouseLocation.Z) / ((int32)SnapSize);

	// 음수일 경우 -1을 더해줌 :
	// 이유 : 음수일 경우 -53도 100으로 나누면 0이 나오기 때문이다. 이때 부터의 기준점은 -100에 Snap이 되어야 한다.
	// X = Sign.X < 0.f ? X - 1 : X;
	// Y = Sign.Y < 0.f ? Y - 1 : Y;
	// Z = Sign.Z < 0.f ? Z - 1 : Z;
	
	// 충돌이 발생했을 때 접촉 면의 Normal을 받아와서 만약 해당 Normal이 -1이면 해당 방향으로 1칸 이동
	if (HitResult.IsValidBlockingHit())
	{
		// Impact Normal도 Round Int를 해야함. 왜냐하면 짤릴 수 있음.
		FIntVector3 ImpactNormal = {
			(int32)FMath::RoundToInt(HitResult.ImpactNormal.X),
			(int32)FMath::RoundToInt(HitResult.ImpactNormal.Y),
			(int32)FMath::RoundToInt(HitResult.ImpactNormal.Z)
		};
		// 충돌이 발생한 Normal을 기준으로 이동
		FIntVector3 Offset = {
			ImpactNormal.X * (int32)FMath::RoundToInt(Size.X),
			ImpactNormal.Y * (int32)FMath::RoundToInt(Size.Y),
			ImpactNormal.Z * (int32)FMath::RoundToInt(Size.Z)
		};
		// X = (int32)ImpactNormal.X < 0.f ? X - Size.X : X + Size.X;
		// Y = (int32)ImpactNormal.Y < 0.f ? Y - Size.Y : Y + Size.Y;
		// Z = (int32)ImpactNormal.Z < 0.f ? Z - Size.Z : Z + Size.Z;
		X += Offset.X;
		Y += Offset.Y;
		Z += Offset.Z;
	}

	// 100 단위 그리드 기준이지만, 블록의 중심을 고려하여 보정
	FVector GridOrigin = FVector(X, Y, Z) * SnapSize;

	
	// FVector CenterOffset = FVector(Size) * SnapSize * 0.5f;
	
	// 물체의 중점을 그리드의 교차점으로 옮김
	FVector CenterOffset = FVector::ZeroVector;
	FVector GridLocation = GridOrigin + CenterOffset;
	
	UpdatedActor->SetActorLocation(GridLocation);

	// 만약 충돌하는 곳이 있다면 해당 장소는 못 간다는 것을 알려줌
	return flag;
}

void UGridComponent::MoveByGizmo(const FVector& NewLocation)
{
	FVector Sign = FVector(1.f, 1.f, 1.f);

	Sign.X = ((int32)NewLocation.X == 0 || (NewLocation.X >= 0.f)) ? 1.f : -1.f;
	Sign.Y = ((int32)NewLocation.Y == 0 || (NewLocation.Y >= 0.f)) ? 1.f : -1.f;
	Sign.Z = ((int32)NewLocation.Z == 0 || (NewLocation.Z >= 0.f)) ? 1.f : -1.f;

	// GridComponent의 위치 계산
	// 그리드의 스냅 단위는 100으로 유지
	X = (int32)FMath::RoundToInt(NewLocation.X) / ((int32)SnapSize);
	Y = (int32)FMath::RoundToInt(NewLocation.Y) / ((int32)SnapSize);
	Z = (int32)FMath::RoundToInt(NewLocation.Z) / ((int32)SnapSize);

	// NOTE: Gizmo로 이동할 때는 -1을 더해주지 않음, 왜냐하면 딱 거기 있어야 하는데 이것 때문에 한번 더 연산이 들어가기 때문이다.
	// X = Sign.X < 0.f ? X - 1 : X;
	// Y = Sign.Y < 0.f ? Y - 1 : Y;
	// Z = Sign.Z < 0.f ? Z - 1 : Z;

	// 100 단위 그리드 기준이지만, 블록의 중심을 고려하여 보정
	FVector GridOrigin = FVector(X, Y, Z) * SnapSize;
	// FVector CenterOffset = FVector(Size) * SnapSize * 0.5f;
	
	// 물체의 중점을 그리드의 교차점으로 옮김
	FVector CenterOffset = FVector::ZeroVector;
	FVector GridLocation = GridOrigin + CenterOffset;
	
	UpdatedActor->SetActorLocation(GridLocation);
}

void UGridComponent::Rotate(const FVector& Direction)
{
	// TODO: 트러블 슈팅 작성하기
	// 이유 : 회전 축을 설정할 때는 월드 좌표계 기준으로 회전해야 한다.

	// 이전 문제
	// FRotator DeltaRotation;
	// DeltaRotation.Roll = Direction.X * RotateAngle;
	// DeltaRotation.Pitch = Direction.Y * RotateAngle;
	// DeltaRotation.Yaw = Direction.Z * RotateAngle;
	//
	// FRotator NewRotation = (CurrentRotation.Quaternion() * DeltaRotation.Quaternion()).Rotator();
	// 이건 로컬 기준 회전 즉, 이전 회전을 기준으로 덧붙이는 거라서,
	// 회전할수록 축이 기울어져서 **의도한 방향(Yaw → 위에서 회전)**이 망가짐.
	
	// 기존 회전
	FQuat CurrentQuat = GetComponentQuat();
	
	// 회전 축 정의 (월드 기준)
	const FVector WorldRight = FVector::RightVector;     // Pitch
	const FVector WorldUp    = FVector::UpVector;        // Yaw
	const FVector WorldForward = FVector::ForwardVector; // Roll
	
	// 각 축 기준으로 회전 쿼터니언 생성 (회전량 * 회전축) : 시계 방향
	FQuat YawQuat   = FQuat(WorldUp,    FMath::DegreesToRadians(Direction.Z * RotateAngle));
	FQuat PitchQuat = FQuat(WorldRight, FMath::DegreesToRadians(Direction.Y * -RotateAngle));
	FQuat RollQuat  = FQuat(WorldForward, FMath::DegreesToRadians(Direction.X * RotateAngle));
	
	// 순서대로 회전 적용 (예: Roll → Pitch → Yaw)
	FQuat CombinedQuat = YawQuat * PitchQuat * RollQuat;
	
	// 월드 축 기준으로 누적 회전 적용
	FQuat NewQuat = CombinedQuat * CurrentQuat;
	
	// Size 스왑은 여전히 필요
	SwapSize(Direction);
	
	// 결과 적용
	UpdatedActor->SetActorRotation(NewQuat);
}

void UGridComponent::SwapSize(const FVector& Direction)
{
	// Roll 회전
	if (Direction == FVector(1, 0, 0))
	{
		int32 Temp = Size.Y;
		Size.Y = Size.Z;
		Size.Z = Temp;
	}
	// Pitch 회전
	else if (Direction == FVector(0, 1, 0))
	{
		int32 Temp = Size.X;
		Size.X = Size.Z;
		Size.Z = Temp;
	}
	// Yaw 회전
	else if (Direction == FVector(0, 0, 1))
	{
		int32 Temp = Size.X;
		Size.X = Size.Y;
		Size.Y = Temp;
	}
}


