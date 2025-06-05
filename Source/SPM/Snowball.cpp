// Fill out your copyright notice in the Description page of Project Settings.


#include "Snowball.h"
#include "CharacterBase.h"
#include "NiagaraFunctionLibrary.h"

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

	//Vad som händer om man träffar just en Player
	if (ACharacterBase* HitCharacter = Cast<ACharacterBase>(OtherActor))
	{
		FVector ImpactDirection = GetVelocity().GetSafeNormal();
		HitCharacter->ApplySnowballHit(Hit,ImpactDirection);
	}

	//Allt som kan träffas av en snöboll Ska ha poof 
	if (SnowPoof)
	{
		FRotator PoofRotation = Hit.ImpactNormal.Rotation(); 
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SnowPoof,
			Hit.ImpactPoint,
			PoofRotation
		);

	}

	
	//Sätt mesh invisible - trigga vfx
	Destroy();
}


/*OM ragdoll någonsin implementeras*/
//RAGDOLL
/*void ACharacterBase::ApplySnowballHit(const FHitResult& Hit, const FVector& ImpactDirection)
{
	if (bIsRagdoll) return;
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	FName BoneName = Hit.BoneName != NAME_None ? Hit.BoneName : FName("spine_03");
	FVector Impulse = ImpactDirection * 5000.0f;
	GetMesh()->AddImpulseAtLocation(Impulse, Hit.ImpactPoint, BoneName);
	
	bIsRagdoll = true;
	
}*/



