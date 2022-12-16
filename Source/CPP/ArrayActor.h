// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseUserWidget.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "ArrayActor.generated.h"

/**
 *
 */
UCLASS()
class CPP_API AArrayActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	AArrayActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PreSaved")
		UWidgetComponent* ShowCardsBlueprintComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PreSaved")
		class TSubclassOf<UUserWidget> ShowCardsBlueprint;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		TArray<UTexture2D*> DiceArray;
	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		int MaxDiceValue = 0;

private:
	virtual void InteractableAction() override;
	virtual void DestroyAction() override;

	UFUNCTION()
		void ShowBoard();
	UFUNCTION()
		void BackToPlayer();
	UFUNCTION()
		void ShuffleDices();

	UFUNCTION(Category = Algo)
		void MaxElement();

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BoardCamera;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Board;

	UPROPERTY(EditAnywhere)
		TMap<UTexture2D*, int> DiceMap;

};
