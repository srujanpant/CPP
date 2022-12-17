// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "CoreMinimal.h"
#include "ESearchType.h"
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
		UWidgetComponent* ShowCardsBlueprintWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PreSaved")
		TSubclassOf<UUserWidget> ShowCardsWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PreSaved")
		UWidgetInteractionComponent* WidgetInteractionComponent;

	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		TArray<UTexture2D*> DiceArray;
	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		int MaxDiceValue = 0;

private:
	virtual void InteractableAction() override;
	virtual void DestroyAction() override;

	UFUNCTION(BlueprintCallable)
		void BackToPlayer();
	UFUNCTION(BlueprintCallable, Category = Algo)
		void SearchDice(int DiceNumber);

	UFUNCTION(Category = Algo)
		void MaxElement();

	UFUNCTION()
		void ShowCards();
	UFUNCTION()
		void ShuffleDices();

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BoardCamera;

	UPROPERTY(EditAnywhere, Category = Algo)
		TEnumAsByte<ESearchType> SearchType;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Board;

	UPROPERTY(EditAnywhere)
		TMap<UTexture2D*, int> DiceMap;

	UPROPERTY(EditAnywhere)
		TArray<int> DiceValues;

	bool bSearchedDiceNumber = false;
};
