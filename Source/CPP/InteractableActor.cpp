// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CPPCharacter.h"
#include "InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetupAttachment(Scene);
	BoxCollision->SetBoxExtent(BoxExtent);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor == Cast<ACPPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		InteractableAction();
	}
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	DestroyAction();
}