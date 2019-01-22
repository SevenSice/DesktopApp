// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter_C.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ACameraActor;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class MotionState : uint8
{
	InSky 		UMETA(DisplayName = "InSky"),
	OnLanded 	UMETA(DisplayName = "OnLanded"),
	Observer	UMETA(DisplayName = "Observer")
};

UCLASS()
class DESKTOPAPP_API AMCharacter_C : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacter_C();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "Move")
		MotionState Move_State;

	//移动的时间
	UPROPERTY(EditAnywhere, Category = "Time")
		float MoveTime;

	//功能触发的CD
	UPROPERTY(EditAnywhere, Category = "Time")
		float MoveCD_Time;

	FTimerHandle TimeHandle_MovePawn;

	//目标点坐标
	FVector TargetLocation;
	//目标点朝向
	FRotator TargetRotation;

	//是否可以位移
	bool bMoving;

	//位移时的时间
	float LastTeleportTime;

	UPROPERTY(EditAnywhere, Category = "Move")
		float TeleportSpeed;

	UPROPERTY(EditAnywhere, Category = "Move")
		float MouseMoveSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void MoveUp(float Value);

	void AddControlPitchInput(float Val);

	void AddControlYawInput(float Val);

	void MouseWheelMove(float Value);

	//切换状态
	void MoveOrFlying();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//触发开始位移
	UFUNCTION(BlueprintCallable, Category = "Move")
		void Move_Start(int Index);

	void Move_Start(FVector MoveTargetLocation, FRotator MoveTargetRotation);

	//每帧位移函数
	UFUNCTION(BlueprintCallable, Category = "Move")
		void SetActorToTargetLocation(FVector TargetLocation, float DeltaTime);

	//每帧旋转角色函数
	UFUNCTION(BlueprintCallable, Category = "Move")
		void SetActorToTargetRotation(FRotator TargetRotation, float DeltaTime);

	//每帧旋转控制器函数
	UFUNCTION(BlueprintCallable, Category = "Move")
		void SetControlToTargetRotation(FRotator TargetRotation, float DeltaTime);

	//每帧旋转相机和相机臂(暂时无效)
	UFUNCTION(BlueprintCallable, Category = "Move")
		void TurnCameraAndArm(FRotator TargetRotation, float DeltaTime);

	//初始化属性
	UFUNCTION(BlueprintCallable, Category = "Move")
		void InitController(float SpringArmLength = 30.0f, FRotator ControlRotator = FRotator(-40.0f, 0, 0), float Camera_FieldOfView = 90.0f);
};
