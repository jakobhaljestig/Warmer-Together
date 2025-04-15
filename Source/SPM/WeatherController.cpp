#include "WeatherController.h"

AWeatherController::AWeatherController()
{
	PrimaryActorTick.bCanEverTick = true;

	WeatherSystem = CreateDefaultSubobject<UAdaptiveWeatherSystem>(TEXT("AdaptiveWeatherSystem"));
}

void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
}

void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeatherController::UpdatePlayerPerformance(const FPerformance& PlayerPerformance)
{
	if (WeatherSystem)
	{
		WeatherSystem->UpdatePerformance(PlayerPerformance);
	}
}

//kan ju kallas i tick men känns överdrivet då den kanske inte behövs så ofta
void AWeatherController::ApplyWeatherToEnvironment()
{
	if (!WeatherSystem) return;

	const FWeatherState& Weather = WeatherSystem->GetCurrentWeather();

	// Här uppdateras snö, vind, dimma osv.
	//SnowParticleSystem->SetFloatParameter("SnowAmount", CurrentWeather.SnowIntensity);
	
	// Ändra global post process
	//PostProcessVolume->Settings.FogDensity = 1.0f - CurrentWeather.Visibility;

}


