// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "HelloWorldActor.generated.h"

/**
 *
 */
UCLASS()
class CPP_API AHelloWorldActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	AHelloWorldActor();

private:
	UPROPERTY()
		UTextRenderComponent* DisplayText;

	UPROPERTY(EditAnywhere, Category = "PreSaved")
		float FontSize = 100.f;

	virtual void InteractableAction() override;
	virtual void DestroyAction() override;
};
