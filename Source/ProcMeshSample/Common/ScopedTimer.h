// Copyright seiroise.

#pragma once

#include "CoreMinimal.h"

class FScopedTimer
{
public:
	FScopedTimer(const FString& InLabel)
		: m_Label(InLabel)
	{
		m_Start = FPlatformTime::Seconds();
	}
	~FScopedTimer()
	{
		double end = FPlatformTime::Seconds();
		UE_LOG(LogTemp, Log, TEXT("%s : %f"), *m_Label, end - m_Start);
	}
	
private:

	FString m_Label;
	double m_Start;
};
