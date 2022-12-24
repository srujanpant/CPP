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
	DiceValues.Empty();
	DiceTextureArray.Empty();
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
		DiceMap.GenerateKeyArray(DiceTextureArray);
		if (DiceTextureArray.Num() > 0)
		{
			for (int i = 0; i < DiceTextureArray.Num(); ++i)
			{
				int Index = FMath::RandRange(i, DiceTextureArray.Num() - 1);
				if (i != Index)
				{
					DiceTextureArray.Swap(i, Index);
				}
			}

			DiceTextureArray.SetNum(4);
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
	for (int i = 0; i < DiceTextureArray.Num(); i++)
	{
		DiceValues.Add(*DiceMap.Find(DiceTextureArray[i]));
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
		SortDices(false);
		bSearchedDiceNumber = BinarySearchFunc(DiceValues, DiceNumber);
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

	default:
		break;
	}
}

void AArrayActor::SortDices(bool bSortDices)
{
	switch (SortType)
	{
	case SelectionSort:
	{
		for (int i = 0; i < DiceValues.Num(); i++)
		{
			auto MinElement = std::min_element(DiceValues.GetData() + i, DiceValues.GetData() + DiceValues.Num());
			std::swap(DiceValues[i], *MinElement);

			if (bSortDices)
			{
				for (auto DiceMapper : DiceMap)
				{
					if (DiceMapper.Value == DiceValues[i])
					{
						DiceTextureArray[i] = DiceMapper.Key;
					}
				}
			}
		}
		break;
	}

	case BubbleSort:
		for (int i = 0; i < DiceValues.Num() - 1; i++)
		{
			bool swapped = false;
			for (int j = 0; j < DiceValues.Num() - 1 - i && swapped; j++)
			{
				if (DiceValues[j] > DiceValues[j + 1])
				{
					std::iter_swap(DiceValues.GetData() + j, DiceValues.GetData() + j + 1);
					swapped = true;
				}
			}
		}

		if (bSortDices)
		{
			for (int i = 0; i < DiceValues.Num(); i++)
			{
				for (auto DiceMappper : DiceMap)
				{
					if (DiceMappper.Value == DiceValues[i])
					{
						DiceTextureArray[i] = DiceMappper.Key;
					}
				}
			}
		}
		break;

	case InsertionSort:
		for (int i = 1; i < DiceValues.Num(); i++)
		{
			int current = DiceValues[i];
			for (int j = i - 1; j >= 0 && DiceValues[j] > current; j--)
			{
				std::iter_swap(DiceValues.GetData() + j, DiceValues.GetData() + j + 1);
			}
		}

		if (bSortDices)
		{
			for (int i = 0; i < DiceValues.Num(); i++)
			{
				for (auto DiceMapper : DiceMap)
				{
					if (DiceMapper.Value == DiceValues[i])
					{
						DiceTextureArray[i] = DiceMapper.Key;
					}
				}
			}
		}
		break;

	default:
		break;
	}
}

bool AArrayActor::BinarySearchFunc(const TArray<int>& Array, int Value)
{
	int Low = 0;
	int High = Array.Num() - 1;
	while (Low <= High)
	{
		int Mid = Low + (High - Low) / 2;
		if (Array[Mid] == Value)
		{
			return true;
		}
		else if (Array[Mid] < Value)
		{
			Low = Mid + 1;
		}
		else
		{
			High = Mid - 1;
		}
	}
	return false;
}

/*

1. Sorted Integer array
2. For each value of array, assign corresponding key texture to dice array

*/