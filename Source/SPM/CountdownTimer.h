#pragma once

class CountdownTimer
{
public:

	CountdownTimer();
	void Start(UWorld* World, float Time, TFunction<void()> Callback);
	void CancelTimer();

private:
	FTimerHandle TimerHandle;
	TFunction<void()> OnFinished;

	void CountdownComplete () const;

	UWorld* WorldContext = nullptr;
	
	
	
};
