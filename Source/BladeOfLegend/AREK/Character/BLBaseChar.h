// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "BLBaseChar.generated.h"

class USphereComponent;

UCLASS()
class BLADEOFLEGEND_API ABLBaseChar : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	ABLBaseChar();

protected:
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> Capsule;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPaperFlipbookComponent> PaperFlipbook;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UPaperFlipbookComponent> CrystalFlipbook;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* TranslucencyHelper;
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* TranslucencyHelperBase;
};
