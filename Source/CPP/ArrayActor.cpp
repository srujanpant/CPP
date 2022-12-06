// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ArrayActor.h"

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
	ShowBoard();
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

	ShowCardsBlueprintComponent->ToggleVisibility(true);
	CastToShowCardsWidget();
	UE_LOG(LogTemp, Warning, TEXT("Hello %d"), Cards.Max());
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