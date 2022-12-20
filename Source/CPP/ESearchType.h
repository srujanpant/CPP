// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ESearchType.generated.h"

/**
 *
 */
UENUM(BlueprintType)

enum ESearchType
{
	LinearSearch		UMETA(DisplayName = "LinearSearch"),
	BinarySearch		UMETA(DisplayName = "BinarySearch")
};