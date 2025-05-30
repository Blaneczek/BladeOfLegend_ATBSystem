// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "BLPlayer.generated.h"

class ABLCompanion;
class UBLHeroDataAsset;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UBLCameraDefault;
class UBLSpringArmDefault;

UCLASS()
class BLADEOFLEGEND_API ABLPlayer : public ABLBaseChar
{
	GENERATED_BODY()

public:
	ABLPlayer();

	virtual void BeginPlay() override;

private:
	void InitializePlayerController();

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	void UpdateFlipbookFromDAHeroes() const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBLSpringArmDefault> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBLCameraDefault> Camera;

#pragma region Companion

public:
	UFUNCTION(BlueprintCallable)
	void AddCompanions();

private:
	UPROPERTY(VisibleAnywhere, Category = "BL|Companion")
	TArray<ABLCompanion*> ActiveCompanions;
	UPROPERTY(VisibleAnywhere, Category = "BL|Companion")
	float LineSpacing{50.f};
	UPROPERTY(EditDefaultsOnly, Category = "BL|Companion")
	TSubclassOf<ABLCompanion> CompanionClass;

#pragma endregion Companion

#pragma region EnhancedInput

public:
	// Setup input actions
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// Setup input mapping context
	void SetupInputMappingContext() const;

	// Set those variables in blueprints
	UPROPERTY(EditDefaultsOnly, Category = "BL|Enhanced Input")
	TObjectPtr<UInputMappingContext> IMC_Default;
	// Input for interaction
	UPROPERTY(EditDefaultsOnly, Category = "BL|Enhanced Input")
	TObjectPtr<UInputAction> IA_Interact;
	// Input for movement
	UPROPERTY(EditDefaultsOnly, Category = "BL|Enhanced Input")
	TObjectPtr<UInputAction> IA_Move;

#pragma endregion EnhancedInput

#pragma region Interaction

public:
	// Bind with IA_Interact
	void OnInteract();

	UFUNCTION()
	void OnInteractSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void BindInteractSphereOverlapFunctions();

	void SetInteractableActor(TObjectPtr<AActor> Actor);
	void ChooseInteractableActor();

	UPROPERTY(VisibleAnywhere, Category = "BL|Interaction")
	TObjectPtr<USphereComponent> InteractSphere;
	UPROPERTY(VisibleAnywhere, Category = "BL|Interaction")
	TObjectPtr<AActor> InteractableActor;
	UPROPERTY(VisibleAnywhere, Category = "BL|Interaction")
	float InteractSphereRadius{64.f};
	UPROPERTY(VisibleAnywhere, Category = "BL|Interaction")
	TArray<TObjectPtr<AActor>> ActorsInInteractSphereArray;

#pragma endregion Interaction

#pragma region Movement

private:
	// Bind with IA_Move
	void OnMove(const FInputActionValue& Value);
	void MoveInDirection(const float X, const float Y);
	void RotateInDirection(const float X, const float Y);

#pragma endregion Movement
};
