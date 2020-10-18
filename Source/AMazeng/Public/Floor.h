// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Floor.generated.h"

UCLASS()
class AMAZENG_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetupFloor(uint32 depth, uint32 floor, uint32 width);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
