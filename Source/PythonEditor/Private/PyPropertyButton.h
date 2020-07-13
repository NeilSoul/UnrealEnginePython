//////////////////////////////////////////////////////////////////////////
// DK Begin: ID(#DK_PyPropertyButton) modifier:(xingtongli)
#pragma once

#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "PyPropertyButton.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FPyPropertyButton
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Python")
	FString ScriptPath;

	UPROPERTY(EditAnywhere, Category = "Python")
	FString Function;
};
// DK End
//////////////////////////////////////////////////////////////////////////

