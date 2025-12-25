// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

#define PRINT_DEBUG_MESSAGE(String) \
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, String); }
