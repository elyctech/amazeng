// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Wall.generated.h"

enum class WallDirection
{
	X,
	Y
};

UCLASS()
class AMAZENG_API AWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetupWall(int32 x, int32 y, uint32 floor, uint32 length, WallDirection direction);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
