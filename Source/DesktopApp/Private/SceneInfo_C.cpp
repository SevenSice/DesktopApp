// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneInfo_C.h"

// Sets default values
ASceneInfo_C::ASceneInfo_C()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;


}

// Called when the game starts or when spawned
void ASceneInfo_C::BeginPlay()
{
	Super::BeginPlay();

}


//返回目标相机位置
FVector ASceneInfo_C::GetTargetActorLocation(int Index)
{
	if (Index <= (GetTargetArrayLength() - 1) && Index >= 0)
	{
		FVector TargetLocation = TargetActor[Index]->GetActorLocation();

		return TargetLocation;
	}
	UE_LOG(LogTemp, Warning, TEXT("Get Target Actor is Null ！"));
	return FVector::ZeroVector;
}

//返回目标相机朝向
FRotator ASceneInfo_C::GetTargetActorRotation(int Index)
{
	if (Index <= (GetTargetArrayLength() - 1) && Index >= 0)
	{
		FRotator TargetRotation = TargetActor[Index]->GetActorRotation();

		return TargetRotation;
	}
	UE_LOG(LogTemp, Warning, TEXT("Get Target Actor is Null ！"));
	return FRotator::ZeroRotator;
}

//返回目标数组长度
int ASceneInfo_C::GetTargetArrayLength()
{
	int length = TargetActor.GetAllocatedSize() / 8;
	return length;
}

