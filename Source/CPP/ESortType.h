// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ESortType.generated.h"

/**
 *
 */
UENUM(BlueprintType)

enum ESortType
{
	SelectionSort		UMETA(DisplayName = "SelectionSort"),
	BubbleSort			UMETA(DisplayName = "BubbleSort"),
	InsertionSort		UMETA(DisplayName = "InsertionSort")
};
