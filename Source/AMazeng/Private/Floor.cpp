// Fill out your copyright notice in the Description page of Project Settings.

#include "Floor.h"

// Sets default values
AFloor::AFloor()
{
	// Disable ticking
	PrimaryActorTick.bCanEverTick = false;

	// Create Cube Mesh Component
	UStaticMeshComponent* floorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	UStaticMesh* floorMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	floorMeshComponent->SetStaticMesh(floorMesh);

	// Set as root component
	RootComponent = floorMeshComponent;
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFloor::SetupFloor(uint32 depth, uint32 floor, uint32 width)
{
	RootComponent->SetWorldScale3D(
		FVector(
			depth,
			width,
			0.2f
		)
	);

	SetActorLocation(
		FVector(
			depth * 50.0f,
			width * 50.0f,
			floor * 400.0f + 10.0f
		)
	);
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

