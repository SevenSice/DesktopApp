// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneInfo_C.generated.h"





UCLASS()
class DESKTOPAPP_API ASceneInfo_C : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneInfo_C();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent *SceneComp;

	UPROPERTY(EditInstanceOnly, Category = "TargetsActor")
		TArray<AActor*> TargetActor;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		FVector GetTargetActorLocation(int Index);

	UFUNCTION()
		FRotator GetTargetActorRotation(int Index);

	UFUNCTION()
		int GetTargetArrayLength();
};
