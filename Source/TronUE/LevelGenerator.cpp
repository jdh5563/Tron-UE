// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelGenerator::BuildLevelOutline(TSubclassOf<AActor> wallBP)
{
	// Build vertical walls
	for (int row = 0; row < gridHeight; row++)
	{
		AActor* wall1 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall1->SetActorLocation(FVector(0, row * 100, 0));

		AActor* wall2 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall2->SetActorLocation(FVector((gridWidth - 1) * 100, row * 100, 0));

		grid[row * gridWidth] = wall1;
		grid[row * gridWidth + gridHeight - 1] = wall2;
	}

	// Build horizontal walls
	for (int col = 1; col < gridWidth - 1; col++)
	{
		AActor* wall1 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall1->SetActorLocation(FVector(col * 100, 0, 0));

		AActor* wall2 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall2->SetActorLocation(FVector(col * 100, (gridHeight - 1) * 100, 0));

		grid[col] = wall1;
		grid[col + (gridWidth * (gridHeight - 1))] = wall2;
	}

	// Create floor
	AActor* floor = GetWorld()->SpawnActor<AActor>(wallBP);
	floor->SetActorLocation(FVector(gridWidth * 50.0 - 50.0, gridHeight * 50.0 - 50.0, -100.0));
	floor->SetActorScale3D(FVector(gridWidth - 2, gridHeight - 2, 1.0));
}

void ALevelGenerator::GenerateObstacles(TSubclassOf<AActor> wallBP)
{
	int numWalls = (gridWidth - 2) * (gridHeight - 2) * wallRatio;

	int minWallLength = 2;
	int minWallHeight = 2;
	int maxWallLength = gridWidth * 0.4f;
	int maxWallHeight = gridHeight * 0.4f;

	while (numWalls > 0)
	{
		int wallLength = FMath::RandRange(minWallLength, maxWallLength);
		int wallHeight = FMath::RandRange(minWallHeight, maxWallHeight);

		//Vector2Int randomPos = new Vector2Int(Random.Range(0, grid.GetLength(0)), Random.Range(0, grid.GetLength(1)));
		//int spawnAttempts = 0;

		//// Only allow the position to be one that wasn't already used to generate a wall and is in-bounds
		//// NOTE: WALLS ARE ALLOWED TO OVERLAP WHEN GENERATED THIS WAY
		//while (grid[randomPos.x, randomPos.y].tag == "Wall" || randomPos.x + wallHeight >= gridHeight || randomPos.y + wallLength >= gridWidth)
		//{
		//	randomPos = new Vector2Int(Random.Range(0, grid.GetLength(0)), Random.Range(0, grid.GetLength(1)));
		//	spawnAttempts++;

		//	// Stop trying after 10 attempts to prevent an infinite loop in the event that a new wall cannot be generated
		//	if (spawnAttempts > 10) return;
		//}

		//// Add the wall to the world, replacing the tiles that already exist
		//for (int i = 0; i < wallHeight; i++)
		//{
		//	for (int j = 0; j < wallLength; j++)
		//	{
		//		Destroy(grid[randomPos.x + i, randomPos.y + j]);

		//		GameObject wall = Instantiate(wallPrefab, new Vector3((randomPos.y + j) * 5, 5, (randomPos.x + i) * 5), Quaternion.identity, wallContainer.transform);
		//		grid[randomPos.x + i, randomPos.y + j] = wall;
		//	}
		//}

		//numWalls -= wallLength * wallHeight;
	}
}

TArray<AActor*> ALevelGenerator::GenerateLevel(TSubclassOf<AActor> wallBP)
{
	// Clear the grid
	grid.Empty();

	// Initialize the array with default values
	for (int i = 0; i < gridWidth * gridHeight; i++)
	{
		grid.Add(nullptr);
	}

	// Build walls
	BuildLevelOutline(wallBP);

	return grid;
}
