// Fill out your copyright notice in the Description page of Project Settings.


#include "Snowball.h"
#include "CharacterBase.h"

// Sets default values
ASnowball::ASnowball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collission setup
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetSphereRadius(20.0f); 
	RootComponent = CollisionComp;

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); 
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetNotifyRigidBodyCollision(true);

	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp -> SetUpdatedComponent(CollisionComp);
	MovementComp-> InitialSpeed = 1500.0f;
	MovementComp-> MaxSpeed = 1500.0f;
	MovementComp -> bRotationFollowsVelocity = true;
	MovementComp -> bShouldBounce = false;
	MovementComp -> ProjectileGravityScale = 1.0f;

	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->OnComponentHit.AddDynamic(this, &ASnowball::OnHit);
}

void ASnowball::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Snöbollens ägare: %s"), *GetOwner()->GetName());
	
	//Gör så snöbollen ignorerar den som kastar
	if (AActor* MyOwner = GetOwner())
	{
		CollisionComp->IgnoreActorWhenMoving(MyOwner, true);
	}

	SetLifeSpan(MaxLifeTime);
}

void ASnowball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnowball::ThrowInDirection(const FVector& ThrowDirection, const float InSpeed) const
{
	if (MovementComp)
	{
		MovementComp->InitialSpeed = InSpeed;
		MovementComp->Velocity = ThrowDirection * InSpeed;
	}
}

void ASnowball::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Snöboll träffade något!"));

	if (OtherActor == GetOwner()) return;

	if (ACharacterBase* HitCharacter = Cast<ACharacterBase>(OtherActor))
	{
		HitCharacter->ApplySnowballHit();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Snöboll förstördes"));
	Destroy();
}





