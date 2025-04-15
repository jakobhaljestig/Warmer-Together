#include "SPMGameInstance.h"

USPMGameInstance::USPMGameInstance()
{
	// Skapa vädersysteminstansen
	WeatherSystem = CreateDefaultSubobject<UAdaptiveWeatherSystem>(TEXT("AdaptiveWeatherSystem"));
}

void USPMGameInstance::Init()
{
	Super::Init();

	// Eventuell initialisering för vädersystemet om det behövs
	if (WeatherSystem)
	{
		// Kan också lägga till eventuella andra initialiseringar här
		UE_LOG(LogTemp, Warning, TEXT("Hello from weather system!"));
	}
}
