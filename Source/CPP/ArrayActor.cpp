// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrayActor.h"
#include "BaseUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/GameplayStatics.h"

AArrayActor::AArrayActor()
{
	Board = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));
	BoardCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BoardCamera"));
	ShowCardsBlueprintComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShowCardsBlueprintComponent"));

	Board->SetupAttachment(Scene);
	BoardCamera->SetupAttachment(Scene);
	ShowCardsBlueprintComponent->SetupAttachment(Board);
	ShowCardsBlueprintComponent->ToggleVisibility(false);
}

void AArrayActor::InteractableAction()
{
	ShuffleDices();
	ShowBoard();
	MaxElement();
}

void AArrayActor::DestroyAction()
{
}

void AArrayActor::ShowBoard()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreMoveInput(true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 0.5f);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	ShowCardsBlueprintComponent->SetWidgetClass(ShowCardsBlueprint);
	ShowCardsBlueprintComponent->ToggleVisibility(true);
}

void AArrayActor::BackToPlayer()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend
	(UGameplayStatics::GetActorOfClass(GetWorld(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->StaticClass()),
		0.5f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreMoveInput(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
}

void AArrayActor::ShuffleDices()
{
	DiceMap.GenerateKeyArray(DiceArray);
	if (DiceArray.Num() > 0)
	{
		for (int i = 0; i < DiceArray.Num(); ++i)
		{
			int Index = FMath::RandRange(i, DiceArray.Num() - 1);
			if (i != Index)
			{
				DiceArray.Swap(i, Index);
			}
		}
	}
	DiceArray.SetNum(4);
}

void AArrayActor::MaxElement()
{
	TArray<int> DiceValues;
	for (int i = 0; i < DiceArray.Num(); i++)
	{
		DiceValues.Add(*DiceMap.Find(DiceArray[i]));
		MaxDiceValue = std::max(MaxDiceValue, DiceValues[i]);
	}

	UE_LOG(LogTemp, Warning, TEXT("Check %d"), MaxDiceValue);
}
