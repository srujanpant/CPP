// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class CPP_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InteractableAction()
		PURE_VIRTUAL(AInteractableActor::InteractableAction, );

	virtual void DestroyAction()
		PURE_VIRTUAL(AInteractableActor::DestroyAction, );

	UPROPERTY()
		USceneComponent* Scene;

private:
	UPROPERTY()
		UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category = "PreSaved")
		FVector BoxExtent = FVector(300.f, 300.f, 200.f);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
