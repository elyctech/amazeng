// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"


// Sets default values
AWall::AWall()
{
	// Disable ticking
	PrimaryActorTick.bCanEverTick = false;

	// Create Cube Mesh Component

	UStaticMeshComponent* wallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	UStaticMesh* wallMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	wallMeshComponent->SetStaticMesh(wallMesh);

	// Set as root component
	RootComponent = wallMeshComponent;
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();

}

void AWall::SetupWall(int32 x, int32 y, uint32 floor, uint32 length, WallDirection direction)
{
	// Coerce the cube into a wall

	RootComponent->SetWorldScale3D(
		FVector(
			0.1f,
			length,
			3.8f
		)
	);

	// Place the wall as intended

	x *= 100;
	y *= 100;

	uint8 rotationFactor = 0;

	if (direction == WallDirection::X)
	{
		x += length * 50.0f;
		rotationFactor = 1;
	}
	else
	{
		y += length * 50.0f;
	}

	SetActorLocationAndRotation(
		FVector(
			x,
			y,
			floor * 400 + 210
		),
		FQuat(
			FVector(
				0.0f,
				0.0f,
				1.0f
			),
			PI / 2.0f * rotationFactor
		)
	);
}


// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

