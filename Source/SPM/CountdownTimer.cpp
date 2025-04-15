#include "CountdownTimer.h"


CountdownTimer::CountdownTimer()
{
	
}

//callback = funktionen som ska utföras efter timern har räknat ner
void CountdownTimer::Start(UWorld* World, float Time, TFunction<void()> Callback)
{
	WorldContext = World;
	OnFinished = Callback;

	if (WorldContext)
	{

		UE_LOG(LogTemp, Display, TEXT("CountdownTimer started: %2f seconds"), Time);

		//CountdownComplete metoden genomförs efter timern är slut. 
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
	//Den lagrade callbackfunktionen 
	OnFinished();
}
