// Fill out your copyright notice in the Description page of Project Settings.

#include "MCharacter_C.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraActor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SceneInfo_C.h"
#include "EngineUtils.h "

// Sets default values
AMCharacter_C::AMCharacter_C()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 30.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->BrakingDecelerationFlying = 10000.0f;
	//GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;

	TargetLocation = GetActorLocation();
	TargetRotation = GetActorRotation();

	Move_State = MotionState::InSky;

	bMoving = false;

	MoveTime = 3.0f;
	MoveCD_Time = MoveTime;
	TeleportSpeed = 3.0f;

	MouseMoveSpeed = 50.0f;
}

// Called when the game starts or when spawned
void AMCharacter_C::BeginPlay()
{
	Super::BeginPlay();

	LastTeleportTime = GetWorld()->TimeSeconds;

	GetWorldTimerManager().SetTimer(TimeHandle_MovePawn, MoveTime, false, 0.0f);

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}


void AMCharacter_C::MoveForward(float Value)
{
	if (Move_State == MotionState::Observer)
	{
		return;
	}
	//AddMovementInput(GetActorForwardVector() * Value);
	//UE_LOG(LogTemp, Warning, TEXT("按下了！"));
	AddMovementInput(CameraComp->GetComponentRotation().Vector() * Value);

}


void AMCharacter_C::MoveRight(float Value)
{
	if (Move_State == MotionState::Observer)
	{
		return;
	}
	AddMovementInput(GetActorRightVector() * Value);
}


void AMCharacter_C::MoveUp(float Value)
{
	//只允许在自由飞行模式使用。
	if (Move_State == MotionState::InSky)
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			//AddMovementInput(FVector(0, 0, 1) *Value);
			AddMovementInput(GetActorUpVector() *Value);
		}
	}


}

//左右旋转
void AMCharacter_C::AddControlPitchInput(float Val)
{
	if (Move_State == MotionState::OnLanded || Move_State == MotionState::InSky)
	{
		APawn::AddControllerPitchInput(Val);
	}
}

//上下旋转
void AMCharacter_C::AddControlYawInput(float Val)
{
	APawn::AddControllerYawInput(Val);
}

//鼠标滚轮移动
void AMCharacter_C::MouseWheelMove(float Value)
{
	if (Value > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

		//UE_LOG(LogTemp, Warning, TEXT("Value = %f"),Value);
		AddMovementInput(CameraComp->GetComponentRotation().Vector() * Value);
	}
	else if (Value == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Value = 0 "));
	}
	else
	{

		//UE_LOG(LogTemp, Warning, TEXT("Click Button Down！"));
	}

}

//切换状态
void AMCharacter_C::MoveOrFlying()
{

	if (Move_State == MotionState::OnLanded)
	{
		Move_State = MotionState::InSky;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	}
	else if (Move_State == MotionState::InSky)
	{
		Move_State = MotionState::OnLanded;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->Mass = 100;
	}
}

/* *********************Called every frame********************* */
void AMCharacter_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetWorld()->TimeSeconds - LastTeleportTime) > MoveTime)
	{
		bMoving = false;
	}

	SetActorToTargetLocation(TargetLocation, DeltaTime);
	SetControlToTargetRotation(TargetRotation, DeltaTime);


}


//触发开始位移
void AMCharacter_C::Move_Start(int Index)
{
	bMoving = true;
	LastTeleportTime = GetWorld()->TimeSeconds;

	TArray<AActor*> ArryActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASceneInfo_C::StaticClass(), ArryActors);
	if (ArryActors.GetAllocatedSize() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ArryActors is Null ! "));
		return;
	}
	TargetLocation = Cast<ASceneInfo_C>(ArryActors[0])->GetTargetActorLocation(Index);
	TargetRotation = Cast<ASceneInfo_C>(ArryActors[0])->GetTargetActorRotation(Index);

	//TActorIterator<ASceneInfo_C> ActorItr(GetWorld());
	//TargetLocation = ActorItr->GetTargetLocation(Index);
	//TargetRotation = ActorItr->GetTargetRotation(Index);

}

void AMCharacter_C::Move_Start(FVector MoveTargetLocation, FRotator MoveTargetRotation)
{
	bMoving = true;
	LastTeleportTime = GetWorld()->TimeSeconds;

	TargetLocation = MoveTargetLocation;
	TargetRotation = MoveTargetRotation;
}

//每帧位移函数
void AMCharacter_C::SetActorToTargetLocation(FVector TargetLocation, float DeltaTime)
{
	if (bMoving == true)
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, TeleportSpeed);

		SetActorLocation(NewLocation);
	}
}

//每帧旋转角色函数
void AMCharacter_C::SetActorToTargetRotation(FRotator TargetRotation, float DeltaTime)
{
	if (bMoving == true)
	{
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, TeleportSpeed);

		SetActorRotation(NewRotation);
	}
}

//每帧旋转控制器函数
void AMCharacter_C::SetControlToTargetRotation(FRotator TargetRotation, float DeltaTime)
{
	if (bMoving == true)
	{
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, TeleportSpeed);

		GetController()->SetControlRotation(NewRotation);

	}
}

//每帧旋转相机和相机臂函数(暂时无效)
void AMCharacter_C::TurnCameraAndArm(FRotator TargetRotation, float DeltaTime)
{
	if (bMoving == true)
	{
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, TeleportSpeed);
		if (NewRotation.Pitch < SpringArmComp->GetComponentRotation().Pitch)
		{
			SpringArmComp->AddWorldRotation(FRotator(1, 0, 0));
		}
		if (NewRotation.Yaw < CameraComp->GetComponentRotation().Yaw)
		{
			CameraComp->AddWorldRotation(FRotator(0, 1, 0));
		}

	}
}

//初始化属性
void AMCharacter_C::InitController(float SpringArmLength, FRotator ControlRotator, float Camera_FieldOfView)
{
	SpringArmComp->TargetArmLength = SpringArmLength;
	GetController()->SetControlRotation(ControlRotator);
	CameraComp->SetFieldOfView(Camera_FieldOfView);
}


// Called to bind functionality to input
void AMCharacter_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter_C::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter_C::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AMCharacter_C::MoveUp);
	PlayerInputComponent->BindAxis("MouseWheelMove", this, &AMCharacter_C::MouseWheelMove);

	PlayerInputComponent->BindAxis("LookUp", this, &AMCharacter_C::AddControlPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMCharacter_C::AddControlYawInput);

	PlayerInputComponent->BindAction("MoveOrFlying", IE_Pressed, this, &AMCharacter_C::MoveOrFlying);

}
