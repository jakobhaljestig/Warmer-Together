#include "C:\Users\novi4600\Documents\GitHub\SPM\Intermediate\Build\Win64\x64\SPMEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "CountdownTimer.h"


CountdownTimer::CountdownTimer()
{
	
}

void CountdownTimer::Start(UWorld* World, float Time, TFunction<void()> Callback)
{
	WorldContext = World;
	OnFinished = Callback;

	if (WorldContext)
	{

		UE_LOG(LogTemp, Display, TEXT("CountdownTimer started: %2f seconds"), Time);
		
		WorldContext -> GetTimerManager().SetTimer(TimerHandle, [this]()->void{CountdownComplete();}, Time, false);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Invalid WorldContext - counter not started"));
	}
}

void CountdownTimer::CancelTimer()
{
	if (WorldContext)
	{
		WorldContext -> GetTimerManager().ClearTimer(TimerHandle);
		UE_LOG(LogTemp, Display, TEXT("Timer canceled"));
		
	}
}

void CountdownTimer::CountdownComplete() const
{
	UE_LOG(LogTemp, Display, TEXT("Timer is finished - executing callback function"));
	OnFinished();
}
