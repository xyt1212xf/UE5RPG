#pragma once
namespace Debug
{
	static void Print(const FString& msg, const FColor& color = FColor::MakeRandomColor(), int32 inKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(inKey, 7.0f, color, msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
		}
	}

	static void Print(const FString& FloatTitle,  float FloatValueToPrint, int32 inKey = -1, const FColor& color = FColor::MakeRandomColor() )
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
			GEngine->AddOnScreenDebugMessage(inKey, 7.0f, color, FinalMsg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}
