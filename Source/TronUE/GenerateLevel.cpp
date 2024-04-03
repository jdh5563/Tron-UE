// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLevel.h"

// Sets default values
AGenerateLevel::AGenerateLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGenerateLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerateLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void TankCode()
{
	/*
	 
	 
	public void Init()
    {
		ResetGrid();

		int timesRun = 1;
		//Debug.Log("Times Reset: " + timesRun);

		while (!IsGridValid())
		{
			ResetGrid();

			timesRun++;
			//Debug.Log("Times Reset: " + timesRun);
		}

		navMesh.GetComponent<NavMeshSurface>().BuildNavMesh();
	}

	// Helper functions
	/// <summary>
	/// Generate a new grid if all enemies are defeated or spacebar is pressed
	/// </summary>
	public void GenerateNewGrid()
    {
		int timesRun = 0;
		do
		{
			ResetGrid();
			timesRun++;
		}
		while (!IsGridValid());
		//Debug.Log("Times Reset: " + timesRun);

		StartCoroutine(RegenerateNavMesh());
	}
	public bool PlayerWon()
	{
		if (grid != null && enemies.Count == 0)
		{
			return true;
		}
		return false;
	}
	/// <summary>
	/// Runs a simplified Dijkstra's Algorithm to determine if the level is valid.
	/// </summary>
	/// <returns>Whether the player is able to move to all enemies.</returns>
	private bool IsGridValid()
	{
		// Enemy positions
		foreach (Vector2Int enemySpawn in enemySpawns)
		{
			// Player position
			Vector2Int startPos = new Vector2Int((int)player.transform.position.z / 5, (int)player.transform.position.x / 5);
			GameObject startTile = grid[startPos.x, startPos.y];

			#region Dijkstra's Algorithm
			List<GameObject> openList = new List<GameObject> { startTile };
			List<GameObject> closedList = new List<GameObject>();

			// Current tile
			GameObject current = grid[startPos.x, startPos.y];
			int currentX;
			int currentY;

			// Run until there are no tiles left to search
			while (openList.Count > 0)
			{
				current = openList[0];
				currentX = (int)current.transform.position.z / 5;
				currentY = (int)current.transform.position.x / 5;

				// If the enemy is reached, we are successful
				if (current == grid[enemySpawn.x, enemySpawn.y])
				{
					break;
				}

				// Check the four cardinal directions
				for (int i = 0; i < 4; i++)
				{
					GameObject endTile;

					switch (i)
					{
						case 0:
							endTile = grid[currentX, currentY + 1];
							break;
						case 1:
							endTile = grid[currentX - 1, currentY];
							break;
						case 2:
							endTile = grid[currentX, currentY - 1];
							break;
						default:
							endTile = grid[currentX + 1, currentY];
							break;
					}

					// Add any unsearched tiles
					if (!openList.Contains(endTile) && !closedList.Contains(endTile) && endTile.tag != "Wall")
					{
						openList.Add(endTile);
					}
				}

				openList.Remove(current);
				closedList.Add(current);
			}

			// If the enemy was never found, we failed
			if (current != grid[enemySpawn.x, enemySpawn.y])
			{
				return false;
			}
			#endregion
		}

		return true;
	}
	/// <summary>
	/// Reset the grid and all entities in it
	/// </summary>
	private void ResetGrid()
	{
		if(grid != null) { Clear(); }
		CreateGrid();
		SpawnPlayer();
		SpawnEnemies();
	}
	
	// Wrappers
	public void Clear()
	{
		DeleteGrid();
		DeletePlayer();
		DeleteEnemies();
	}

	// Core functions
	private IEnumerator RegenerateNavMesh()
	{
		navMesh.GetComponent<NavMeshSurface>().RemoveData();

		yield return navMesh.GetComponent<NavMeshSurface>().UpdateNavMesh(navMesh.GetComponent<NavMeshSurface>().navMeshData);

		navMesh.GetComponent<NavMeshSurface>().BuildNavMesh();
	}
	/// <summary>
	/// Create a new grid, then place walls in it
	/// </summary>
	private void CreateGrid()
    {
		// Choose a random size for the grid
		gridWidth = Random.Range(minGridLength, maxGridLength);
		gridHeight = Random.Range(minGridHeight, maxGridHeight);

		// Instantiate tiles in each grid space
		grid = new GameObject[gridHeight, gridWidth];

		for (int i = 0; i < grid.GetLength(0); i++)
		{
			for (int j = 0; j < grid.GetLength(1); j++)
			{
				GameObject tile = i == 0 || i == grid.GetLength(0) - 1 || j == 0 || j == grid.GetLength(1) - 1 ?
				Instantiate(borderPrefab, new Vector3(j * 5, 5, i * 5), Quaternion.identity, borderContainer.transform)
				:
				Instantiate(tilePrefab, new Vector3(j * 5, 0, i * 5), Quaternion.identity, groundContainer.transform);
				grid[i, j] = tile;
			}
		}

		// Choose a random number of walls
		// This number is the number of individual wall tiles, not wall groups
		int numWalls = Random.Range(minNumWalls, maxNumWalls);

		// Add walls until we reach or exceed our maximum
		while(numWalls > 0)
		{
			// Choose a random length, width, and position for the wall
			int wallLength = Random.Range(minWallLength, maxWallLength);
			int wallHeight = Random.Range(minWallHeight, maxWallHeight);

			Vector2Int randomPos = new Vector2Int(Random.Range(0, grid.GetLength(0)), Random.Range(0, grid.GetLength(1)));
			int spawnAttempts = 0;

			// Only allow the position to be one that wasn't already used to generate a wall and is in-bounds
			// NOTE: WALLS ARE ALLOWED TO OVERLAP WHEN GENERATED THIS WAY
			while (grid[randomPos.x, randomPos.y].tag == "Wall" || randomPos.x + wallHeight >= gridHeight || randomPos.y + wallLength >= gridWidth)
			{
				randomPos = new Vector2Int(Random.Range(0, grid.GetLength(0)), Random.Range(0, grid.GetLength(1)));
				spawnAttempts++;

				// Stop trying after 10 attempts to prevent an infinite loop in the event that a new wall cannot be generated
				if (spawnAttempts > 10) return;
			}

			// Add the wall to the world, replacing the tiles that already exist
			for(int i = 0; i < wallHeight; i++)
			{
				for(int j = 0; j < wallLength; j++)
				{
					Destroy(grid[randomPos.x + i, randomPos.y + j]);

					GameObject wall = Instantiate(wallPrefab, new Vector3((randomPos.y + j) * 5, 5, (randomPos.x + i) * 5), Quaternion.identity, wallContainer.transform);
					grid[randomPos.x + i, randomPos.y + j] = wall;
				}
			}

			numWalls -= wallLength * wallHeight;
		}

		Camera.main.transform.SetPositionAndRotation(new Vector3(gridWidth * 5 / 2f, 100f, gridHeight * 5 / 2f), Quaternion.Euler(90, 0, 0));
	}
	/// <summary>
	/// Destroy the current grid
	/// </summary>
	private void DeleteGrid()
	{
		for (int i = 0; i < grid.GetLength(0); i++)
		{
			for (int j = 0; j < grid.GetLength(1); j++)
			{
				Destroy(grid[i, j]);
			}
		}
	}


	*/
}