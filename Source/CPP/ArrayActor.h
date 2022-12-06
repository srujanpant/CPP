// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UFUNCTION(BlueprintImplementableEvent)
		void CastToShowCardsWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PreSaved")
		TArray<UTexture2D*> Cards;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PreSaved")
		UWidgetComponent* ShowCardsBlueprintComponent;

private:
	virtual void InteractableAction() override;
	virtual void DestroyAction() override;

	UFUNCTION()
		void ShowBoard();
	UFUNCTION()
		void BackToPlayer();

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BoardCamera;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Board;
};
