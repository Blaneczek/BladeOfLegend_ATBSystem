// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "BLNPC.generated.h"

UCLASS()
class BLADEOFLEGEND_API ABLNPC : public ABLBaseChar
{
	GENERATED_BODY()

#pragma region Name

public:
	FText GetCharacterName() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="BL|Name")
	FText CharacterName{FText::FromString("NPC")};
	
#pragma endregion Name
};
