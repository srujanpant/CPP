// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloWorldActor.h"

AHelloWorldActor::AHelloWorldActor()
{
	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
	DisplayText->SetupAttachment(Scene);

	DisplayText->SetVisibility(false);
	DisplayText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
}

void AHelloWorldActor::InteractableAction()
{
	DisplayText->SetVisibility(true);
	DisplayText->SetWorldSize(FontSize);
	DisplayText->SetText(FText::FromString("Hello World"));
}

void AHelloWorldActor::DestroyAction()
{
	DisplayText->SetVisibility(false);
}
