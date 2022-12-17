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
	ShowCardsBlueprintWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShowCardsBlueprintWidgetComponent"));
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent >(TEXT("WidgetInteractionComponent"));

	Board->SetupAttachment(Scene);
	BoardCamera->SetupAttachment(Scene);
	ShowCardsBlueprintWidgetComponent->SetupAttachment(Board);
	ShowCardsBlueprintWidgetComponent->ToggleVisibility(false);
	WidgetInteractionComponent->InteractionSource = EWidgetInteractionSource::Mouse;

	//Set Widget RecieveHardwareInput as true in Blueprint since its not possble to do it in C++ wihtout inheriting UWidgetInteractionComponent
}

void AArrayActor::InteractableAction()
{
	ShuffleDices();
	ShowCards();
}

void AArrayActor::DestroyAction()
{
	ShowCardsBlueprintWidgetComponent->ToggleVisibility(false);
	MaxDiceValue = 0;
}

void AArrayActor::ShowCards()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreMoveInput(true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 0.5f);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	ShowCardsBlueprintWidgetComponent->SetWidgetClass(ShowCardsWidget);
	ShowCardsBlueprintWidgetComponent->ToggleVisibility(true);
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

	MaxElement();
}

void AArrayActor::MaxElement()
{
	for (int i = 0; i < DiceArray.Num(); i++)
	{
		DiceValues.Add(*DiceMap.Find(DiceArray[i]));
		MaxDiceValue = std::max(MaxDiceValue, DiceValues[i]);
	}
}

void AArrayActor::SearchDice(int DiceNumber)
{
	switch (SearchType)
	{
	case LinearSearch:
		for (int i = 0; i < DiceValues.Num(); i++)
		{
			if (DiceValues[i] == DiceNumber)
			{
				bSearchedDiceNumber = true;
			}
		}
		if (bSearchedDiceNumber)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dice Number found"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Dice not found"));
		}
		break;
	case BinarySearch:
		break;
	default:
		break;
	}
}