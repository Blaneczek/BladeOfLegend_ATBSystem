// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class BLADEOFLEGEND_API IInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="BL|Interaction")
	void InteractionStart(AActor* OtherInstigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="BL|Interaction")
	void InteractionEnd();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="BL|Interaction")
	void SetInteractableState(bool bCanShowInteract);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="BL|Interaction")
	AActor* GetInteractionInstigator();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="BL|Interaction")
	void SetIsInInteractSphere(bool bIsInInteractSphere);
};
