// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class TRONUE_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	int gridWidth = 25;
	UPROPERTY(EditAnywhere)
	int gridHeight = 25;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float wallRatio = 0.2f;

	TArray<AActor*> grid;

	/// <summary>
	/// Builds the floor and walls of the level
	/// </summary>
	/// <param name="wallBP">A class reference for the walls of the level</param>
	void BuildLevelOutline(TSubclassOf<AActor> wallBP);

	/// <summary>
	/// Generates obstacles within the level
	/// </summary>
	/// <param name="wallBP">A class reference for the walls of the level</param>
	void GenerateObstacles(TSubclassOf<AActor> wallBP);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GenerateLevel(TSubclassOf<AActor> wallBP);
};
