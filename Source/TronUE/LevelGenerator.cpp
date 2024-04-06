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
	for (int row = 0; row < gridHeight; row++)
	{
		AActor* wall1 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall1->SetActorLocation(FVector(0, row * 100, 0));

		AActor* wall2 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall2->SetActorLocation(FVector((gridWidth - 1) * 100, row * 100, 0));

		grid[row * gridWidth] = wall1;
		grid[row * gridWidth + gridHeight - 1] = wall2;
	}

	for (int col = 1; col < gridWidth - 1; col++)
	{
		AActor* wall1 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall1->SetActorLocation(FVector(col * 100, 0, 0));

		AActor* wall2 = GetWorld()->SpawnActor<AActor>(wallBP);
		wall2->SetActorLocation(FVector(col * 100, (gridHeight - 1) * 100, 0));

		grid[col] = wall1;
		grid[col + (gridWidth * (gridHeight - 1))] = wall2;
	}

	AActor* floor = GetWorld()->SpawnActor<AActor>(wallBP);
	floor->SetActorLocation(FVector(gridWidth * 50.0 - 50.0, gridHeight * 50.0 - 50.0, -100.0));
	floor->SetActorScale3D(FVector(gridWidth - 2, gridHeight - 2, 1.0));

	return grid;
}
