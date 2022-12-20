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
	CreateSubObject();
	SetUpSubObjects();
	DiceFoundVisibility = ESlateVisibility::Hidden;
	DiceNotFoundVisibility = ESlateVisibility::Hidden;
	//Set Widget RecieveHardwareInput as true in Blueprint since its not possble to do it in C++ wihtout inheriting UWidgetInteractionComponent
}

void AArrayActor::CreateSubObject()
{
	Board = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));
	BoardCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BoardCamera"));
	ShowCardsBlueprintWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShowCardsBlueprintWidgetComponent"));
	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent >(TEXT("WidgetInteractionComponent"));
}

void AArrayActor::SetUpSubObjects()
{
	Board->SetupAttachment(Scene);
	BoardCamera->SetupAttachment(Scene);
	ShowCardsBlueprintWidgetComponent->SetupAttachment(Board);
	ShowCardsBlueprintWidgetComponent->SetRelativeTransform(FTransform(FRotator(90, 0, 180), FVector(0, 0, 5), FVector(0.15, 0.15, 0.15)));
	ShowCardsBlueprintWidgetComponent->SetDrawSize(FVector2D(1920, 1080));
	ShowCardsBlueprintWidgetComponent->ToggleVisibility(false);
	WidgetInteractionComponent->InteractionSource = EWidgetInteractionSource::Mouse;
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
	DiceFoundVisibility = ESlateVisibility::Hidden;
	DiceNotFoundVisibility = ESlateVisibility::Hidden;
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
	if (DiceMap.Num() > 0)
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

			DiceArray.SetNum(4);
			MaxElement();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("DiceArray null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DiceMap null"));
	}
}

void AArrayActor::MaxElement()
{
	for (int i = 0; i < DiceArray.Num(); i++)
	{
		DiceValues.Add(*DiceMap.Find(DiceArray[i]));
		MaxDiceValue = std::max(MaxDiceValue, DiceValues[i]);
	}
}

void AArrayActor::SearchForDice(int DiceNumber)
{
	DiceFoundVisibility = ESlateVisibility::Hidden;
	DiceNotFoundVisibility = ESlateVisibility::Hidden;

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
			DiceFoundVisibility = ESlateVisibility::Visible;
			bSearchedDiceNumber = false;
		}
		else
		{
			DiceNotFoundVisibility = ESlateVisibility::Visible;
		}
		break;

	case BinarySearch:
		SortDices();
		break;

	default:
		break;
	}
}

void AArrayActor::SortDices()
{
	switch (SortType)
	{
	case SelectionSort:
		for (int i = 0; i < DiceValues.Num(); i++)
		{
			for (int j = i + 1; j < DiceValues.Num(); j++)
			{
				if (DiceValues[j] < DiceValues[i])
				{
					int temp = DiceValues[j];
					DiceValues[j] = DiceValues[i];
					DiceValues[i] = temp;
				}
			}
		}
		break;

	case BubbleSort:
		break;

	case InsertionSort:
		break;
	default:
		break;
	}
}
