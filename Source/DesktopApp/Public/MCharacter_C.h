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

	//�ƶ���ʱ��
	UPROPERTY(EditAnywhere, Category = "Time")
		float MoveTime;

	//���ܴ�����CD
	UPROPERTY(EditAnywhere, Category = "Time")
		float MoveCD_Time;

	FTimerHandle TimeHandle_MovePawn;

	//Ŀ�������
	FVector TargetLocation;
	//Ŀ��㳯��
	FRotator TargetRotation;

	//�Ƿ����λ��
	bool bMoving;

	//λ��ʱ��ʱ��
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

	//�л�״̬
	void MoveOrFlying();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//������ʼλ��
	UFUNCTION(BlueprintCallable, Category = "Move")
		void Move_Start(int Index);

	void Move_Start(FVector MoveTargetLocation, FRotator MoveTargetRotation);

	//ÿ֡λ�ƺ���
	UFUNCTION(BlueprintCallable, Category = "Move")
		void SetActorToTargetLocation(FVector TargetLocation, float DeltaTime);

	//ÿ֡��ת��ɫ����
	UFUNCTION(BlueprintCallable, Category = "Move")
		void SetActorToTargetRotation(FRotator TargetRotation, float DeltaTime);

	//ÿ֡��ת����������
	UFUNCTION(BlueprintCallable, Category = "Move")
		void SetControlToTargetRotation(FRotator TargetRotation, float DeltaTime);

	//ÿ֡��ת����������(��ʱ��Ч)
	UFUNCTION(BlueprintCallable, Category = "Move")
		void TurnCameraAndArm(FRotator TargetRotation, float DeltaTime);

	//��ʼ������
	UFUNCTION(BlueprintCallable, Category = "Move")
		void InitController(float SpringArmLength = 30.0f, FRotator ControlRotator = FRotator(-40.0f, 0, 0), float Camera_FieldOfView = 90.0f);
};
