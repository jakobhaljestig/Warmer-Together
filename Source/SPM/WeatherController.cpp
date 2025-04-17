#include "WeatherController.h"
#include "AdaptiveWeatherSystem.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AWeatherController::AWeatherController()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	SnowParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SnowParticleSystem"));
	SnowParticleSystem -> SetupAttachment(RootComponent);
	SnowParticleSystem -> bAutoActivate = false;
}

void AWeatherController::BeginPlay()
{
	Super::BeginPlay();

	// Hämta vädersystemet från GameInstance (som jag creata asså beast)
	WeatherSystem = GetWorld()->GetGameInstance()->GetSubsystem<UAdaptiveWeatherSystem>();

	if (WeatherSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeatherSystem instansierad korrekt!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeatherSystem kunde inte hittas!"));
	}

	EnableInput(GetWorld()->GetFirstPlayerController());

	
}

void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Här kan man applicera väderförändringar på miljön baserat på vädersystemet
	ApplyWeatherToEnvironment();
}

void AWeatherController::UpdatePlayerPerformance(const FPerformance& PlayerPerformance)
{
	if (WeatherSystem)
	{
		WeatherSystem->UpdatePerformance(PlayerPerformance);
		ApplyWeatherToEnvironment(); // ← uppdatera direkt
	}
}


void AWeatherController::SimulateBadPerformance()
{

	UE_LOG(LogTemp, Warning, TEXT("[WeatherController] SimulateBadPerformance CALLED"));

	// Kolla om det är AI eller manuell anrop
	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Called by: %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Called directly or from AI/BT!"));
	}
	
	if (!WeatherSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("WeatherSystem is NULL in SimulateBadPerformance"));
		return;
	}

	FPerformance BadPerf;
	BadPerf.DeathCount = 5;  // Flera dödsfall
	BadPerf.AveragePuzzleTime = 50.0f;  // Längre tid för att lösa pussel
	BadPerf.TimeNearHeat = 0.0f;  // Liten tid nära värmekälla
	
	if (SnowParticleSystem)
	{
		SnowParticleSystem -> Activate();
	}
	
	WeatherSystem->UpdatePerformance(BadPerf);
	UE_LOG(LogTemp, Warning, TEXT("Simulated bad performance sent to WeatherSystem."));
}


void AWeatherController::SimulateGoodPerformance()
{
	if (WeatherSystem)
	{
		FPerformance GoodPerf;
		GoodPerf.DeathCount = 0;  // Inget dödsfall
		GoodPerf.AveragePuzzleTime = 10.0f;  // Snabb tid för att lösa pussel
		GoodPerf.TimeNearHeat = 999.0f;  // Mycket tid nära värmekälla

		if (SnowParticleSystem)
		{
			SnowParticleSystem -> Deactivate();
		}

		WeatherSystem->UpdatePerformance(GoodPerf);
		UE_LOG(LogTemp, Warning, TEXT("Simulated good performance sent to WeatherSystem."));
	}
}

void AWeatherController::ApplyWeatherToEnvironment() const
{
	if (!WeatherSystem || !FogActor) return;

	const FWeatherState& Weather = WeatherSystem->GetCurrentWeather();

	// Justera dimtäthet (FogDensity)
	UExponentialHeightFogComponent* FogComponent = FogActor->GetComponentByClass<UExponentialHeightFogComponent>();
	if (FogComponent)
	{
		float NewFogDensity = FMath::Clamp(1.0f - Weather.Visibility, 0.1f, 5.0f);  // Justera för att undvika 0.0
		FogComponent->SetFogDensity(NewFogDensity);
	}

}









