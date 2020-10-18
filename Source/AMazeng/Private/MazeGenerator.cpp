// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

#include <set>
#include <utility>
#include <vector>

// TODO Remove once wall merging algorithm is solved
#include <sstream>

#include "Floor.h"
#include "Wall.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
	// Disable ticking
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMazeGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* world = GetWorld();

	uint32 depth = 10;
	uint32 width = 10;

	// Hold walls locations

	// All the walls going in the X direction
	std::vector<std::vector<uint32>> xWalls(width + 1, std::vector<uint32>(depth + 1, 0));

	// Left and right sides are completely walled
	xWalls[0] = std::vector<uint32>(depth + 1, 2);
	xWalls[0][0] = 1;
	xWalls[0][depth] = 1;

	xWalls[width] = std::vector<uint32>(depth + 1, 2);
	xWalls[width][0] = 1;
	xWalls[width][depth] = 1;

	// All the walls going in the Y direction
	std::vector<std::vector<uint32>> yWalls(depth + 1, std::vector<uint32>(width + 1, 0));

	// Top and bottom sides are completely walled
	yWalls[0] = std::vector<uint32>(width + 1, 2);
	yWalls[0][0] = 1;
	yWalls[0][width] = 1;

	yWalls[depth] = std::vector<uint32>(width + 1, 2);
	yWalls[depth][0] = 1;
	yWalls[depth][width] = 1;

	// Enumerate each point a wall can start
	std::set<std::pair<uint32, uint32>> wallPoints;

	for (uint32 x = 1; x < depth; x += 1)
	{
		for (uint32 y = 1; y < width; y += 1)
		{
			wallPoints.insert({ x, y });
		}
	}

	// Generate walls

	while (!wallPoints.empty())
	{
		// Pick a starting point
		std::pair<uint32, uint32> start = *(wallPoints.begin());
		wallPoints.erase(wallPoints.begin());

		uint32 x = start.first;
		uint32 y = start.second;

		if (xWalls[y][x] || yWalls[x][y])
		{
			UE_LOG(LogTemp, Warning, TEXT("Repeating wall at %d, %d"), x, y);
		}

		bool wallHit = false;

		// Keep track of the wall so that it does not run into itself
		std::set<std::pair<uint32, uint32>> wallPath;

		do
		{
			// If a random true comes back, move x. Otherwise, move y
			if (FMath::RandBool())
			{
				// Determine which way to move
				uint32 nextX = FMath::RandBool() ? x + 1 : x - 1;

				// Is there a wall where we are going?
				wallHit = xWalls[y][nextX] || yWalls[nextX][y];

				// The next step of the wall
				std::pair<uint32, uint32> wallStep(nextX, y);

				// Do not let the wall collide with itself
				if (!wallHit || wallPath.find(wallStep) == wallPath.end())
				{
					// TODO Remove once wall merging algorithm is solved
					AWall* wall = GetWorld()->SpawnActor<AWall>();
					wall->SetupWall(FMath::Min(x, nextX), y, 0, 1, WallDirection::X);
					FVector wallLocation = wall->GetActorLocation();
					wall->SetActorLocation(
						FVector(
							wallLocation.X,
							wallLocation.Y - 1100.0f,
							wallLocation.Z
						)
					);

					// Set points as containing a wall
					xWalls[y][x] += 1;
					xWalls[y][nextX] += 1;

					// Add the wall step to the path
					wallPath.emplace(wallStep);

					// Remove the wall step from the points to start from
					wallPoints.erase(wallStep);

					// Advance the wall point
					x = nextX;
				}
			}
			else
			{
				// Determine which way to move
				uint32 nextY = FMath::RandBool() ? y + 1 : y - 1;

				// Is there a wall where we are going?
				wallHit = yWalls[x][nextY] || xWalls[nextY][x];

				// The next step of the wall
				std::pair<uint32, uint32> wallStep(x, nextY);

				// Do not let the wall collide with itself
				if (!wallHit || wallPath.find(wallStep) == wallPath.end())
				{
					// TODO Remove once wall merging algorithm is solved
					//AWall* wall = GetWorld()->SpawnActor<AWall>();
					//wall->SetupWall(x, FMath::Min(y, nextY), 0, 1, WallDirection::Y);
					//FVector wallLocation = wall->GetActorLocation();
					//wall->SetActorLocation(
					//	FVector(
					//		wallLocation.X,
					//		wallLocation.Y - 1100.0f,
					//		wallLocation.Z
					//	)
					//);

					// Set points as containing a wall
					yWalls[x][y] += 1;
					yWalls[x][nextY] += 1;

					// Add the wall step to the path
					wallPath.emplace(wallStep);

					// Remove the wall step from the points to start from
					wallPoints.erase(wallStep);

					// Advance the wall point
					y = nextY;
				}
			}

			wallPath.clear();
		}
		while (!wallHit);
	}

	// Spawn floor

	AFloor* floor1 = world->SpawnActor<AFloor>();
	floor1->SetupFloor(depth, 0, width);

	// Spawn walls

	// TODO Remove once wall merging algorithm is solved
	AFloor* floor2 = world->SpawnActor<AFloor>();
	floor2->SetupFloor(depth, 0, width);
	floor2->SetActorLocation(
		FVector(
			depth * 50.0f,
			width * 50.0f - 1100.0f,
			10.0f
		)
	);

	for (auto walls : xWalls)
	{
		std::stringstream wallString;

		for (uint32 wall : walls)
		{
			if (wall == 0)
			{
				wallString << " ";
			}
			else if (wall == 1)
			{
				wallString << "|";
			}
			else if (wall == 2)
			{
				wallString << "-";
			} 
			else
			{
				wallString << "?";
			}
		}

		FString wallFString(wallString.str().c_str());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *wallFString);
	}

	//return;

	// X Walls
	for (uint32 y = 0; y <= width; y += 1)
	{
		std::vector<uint32> walls = xWalls[y];

		for (uint32 x = 0; x <= depth; x += 1)
		{
			uint32 length = 0;

			while (walls[x] == 0 && x <= depth)
			{
				x += 1;
			}

			uint32 startX = x;

			// Skip the start "1"
			x += 1;

			while (walls[x] > 1 && x <= depth)
			{
				length += 1;
				x += 1;
			}

			// If we ended at the end of a wall, spawn it
			if (walls[x] == 1)
			{
				// Add one to account for the end "1"
				AWall* wall = GetWorld()->SpawnActor<AWall>();
				wall->SetupWall(startX, y, 0, length + 1, WallDirection::X);
			}
		}
	}

	// TODO Remove when X walls are sovled
	return;

	// Y Walls
	for (uint32 x = 0; x <= depth; x += 1)
	{
		std::vector<uint32> walls = yWalls[x];

		for (uint32 y = 0; y <= width; y += 1)
		{
			uint32 length = 0;

			while (walls[y] == 0 && y <= width)
			{
				y += 1;
			}

			uint32 startY = y;

			// Skip the start "1"
			y += 1;

			while (walls[y] > 1 && y <= width)
			{
				length += 1;
				y += 1;
			}

			// If we ended at the end of a wall, spawn it
			if (walls[y] == 1)
			{
				// Add one to account for the end "1"
				AWall* wall = GetWorld()->SpawnActor<AWall>();
				wall->SetupWall(x, startY, 0, length + 1, WallDirection::Y);
			}
		}
	}
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

