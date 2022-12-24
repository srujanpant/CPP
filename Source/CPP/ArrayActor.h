// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "CoreMinimal.h"
#include "ESearchType.h"
#include "ESortType.h"
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetVariables")
		TSubclassOf<UUserWidget> ShowCardsWidget;
	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		TArray<UTexture2D*> DiceTextureArray;
	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		int MaxDiceValue = 0;
	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		ESlateVisibility DiceFoundVisibility;
	UPROPERTY(BlueprintReadWrite, Category = "WidgetVariables")
		ESlateVisibility DiceNotFoundVisibility;

private:
	virtual void InteractableAction() override;
	virtual void DestroyAction() override;

	UFUNCTION(BlueprintCallable, Category = "WidgetFunctions")
		void BackToPlayer();
	UFUNCTION(BlueprintCallable, Category = "WidgetFunctions")
		void SearchForDice(int DiceNumber);
	UFUNCTION(BlueprintCallable, Category = "WidgetFunctions")
		void SortDices(bool bSortDices);

	UFUNCTION(Category = Algo)
		void MaxElement();

	void ShowCards();
	void ShuffleDices();
	void CreateSubObject();
	void SetUpSubObjects();
	bool BinarySearchFunc(const TArray<int>& Array, int Value);

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BoardCamera;

	UPROPERTY(EditAnywhere, Category = Algo)
		TEnumAsByte<ESearchType> SearchType;
	UPROPERTY(EditAnywhere, Category = Algo)
		TEnumAsByte<ESortType> SortType;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Board;
	UPROPERTY(EditAnywhere)
		TMap<UTexture2D*, int> DiceMap;
	UPROPERTY(EditAnywhere)
		UWidgetComponent* ShowCardsBlueprintWidgetComponent;
	UPROPERTY(EditAnywhere)
		UWidgetInteractionComponent* WidgetInteractionComponent;

	TArray<int> DiceValues;
	bool bSearchedDiceNumber = false;
	bool bSortedArray = false;
};
