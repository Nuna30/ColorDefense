// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainGenerator.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ATerrainGenerator::ATerrainGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateHeightMap(MapZ * 2);
	GenerateTerrain();
	GenerateEnvironment();
	TeleportPlayerToLocation(FVector(200 * MapX / 2, 200 * MapY / 2, 200 * MapZ));
}

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerrainGenerator::GenerateEnvironment()
{
	TArray<FString> Grasses = {
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_MCGrass0.SM_MCGrass0"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_MCGrass0.SM_MCGrass1"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_MCGrass0.SM_MCGrass2"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_MCGrass0.SM_MCGrass3"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_MCGrass0.SM_MCGrass4")
	};

	TArray<FString> Flowers = {
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Lily0.SM_Lily0"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Lily0.SM_Lily1"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Lily0.SM_Lily2"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Marigold0.SM_Marigold0"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Marigold0.SM_Marigold1"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Marigold0.SM_Marigold2"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Pansy0.SM_Pansy0"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Pansy0.SM_Pansy1"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Pansy0.SM_Pansy2"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Petunia0.SM_Petunia0"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Petunia0.SM_Petunia1"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Petunia0.SM_Petunia2"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Salvia0.SM_Salvia0"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Salvia0.SM_Salvia1"),
		FString("/Game/MyAssets/Environment/StaticMeshes/SM_Salvia0.SM_Salvia2")
	};

	PlaceEnvironment(Grasses, 50, 75, FVector(200, 200, 100), FRotator(0, 0, 90), FVector(0.8));
	PlaceEnvironment(Flowers, 75, 75, FVector(200, 200, 100), FRotator(0, 0, 90), FVector(0.8));
}

void ATerrainGenerator::PlaceEnvironment(const TArray<FString>& ObjPathList, int32 Percent, int32 OffsetZ, const FVector& BlockSize, const FRotator& Rotation, const FVector& Scale)
{
	if (Percent < 0) Percent = 0;
	else if (Percent >= 100) Percent = 50;
	
	int32 GrassCount = MapX * MapY * Percent / 100;
	for (int32 g = 0; g < GrassCount; g++)
	{
		int32 RandX = FMath::RandRange(0, MapX - 1);
		int32 RandY = FMath::RandRange(0, MapY - 1);
		int32 RandRY = FMath::RandRange(0, 360);
		int32 Z = HeightMap[RandX][RandY];

		int32 RandN = FMath::RandRange(0, ObjPathList.Num() - 1);
		FString ObjPath = ObjPathList[RandN];

		SpawnStaticMeshByPath(ObjPath, FVector(BlockSize.X * RandX, BlockSize.Y * RandY, BlockSize.Z * Z + OffsetZ), FRotator(Rotation.Pitch, Rotation.Yaw + RandRY, Rotation.Roll), Scale, false);
	}

}

void ATerrainGenerator::GenerateTerrain() 
{
	int32 lx = 200, ly = 200, lz = 100;
	FString Plain_GB_0 = FString("/Game/MyAssets/Terrain/StaticMeshes/SM_Plain_GB_0.SM_Plain_GB_0");
	FString Plain_DB   = FString("/Game/MyAssets/Terrain/StaticMeshes/SM_Plain_DB.SM_Plain_DB");
	
	TArray<TArray<TArray<FString>>> Map;

	// HeightMap에 따라 바닥 지형 생성
	Map.SetNum(MapX);
	for (int32 X = 0; X < MapX; ++X)
	{
		Map[X].SetNum(MapY); 
		for (int32 Y = 0; Y < MapY; ++Y)
		{
			// 맨 위 블록은 잔디, 아래는 흙 블록으로 생성
			Map[X][Y].SetNum(MapZ); 
			Map[X][Y][HeightMap[X][Y]] = Plain_GB_0;
			for (int32 Z = 0; Z < HeightMap[X][Y]; ++Z)
			{
				Map[X][Y][Z] = Plain_DB;
			}
		}
	}

	for (int32 X = 0; X < MapX; ++X)
	{
		for (int32 Y = 0; Y < MapY; ++Y)
		{
			for (int32 Z = 0; Z < MapZ; ++Z)
			{
				FString ObjName = Map[X][Y][Z];
				if (ObjName != FString(""))
				{
					SpawnStaticMeshByPath(Map[X][Y][Z], FVector(lx * X, ly * Y, lz * Z), FRotator::ZeroRotator, FVector(1, 1, 1), true);
				}
			}
		}
	}
}

void ATerrainGenerator::GenerateHeightMap(int32 StackCount) 
{
	HeightMap.SetNum(MapX);
	for (int32 x = 0; x < MapX; x++) 
	{
		HeightMap[x].SetNum(MapY);
		for (int32 y = 0; y < MapY; y++)
		{
			HeightMap[x][y] = 0;
		}
	}
	
	
	for (int32 s = 0; s < StackCount; s++)
	{
		int32 RandX = FMath::RandRange(0, MapX - 1);
		int32 RandY = FMath::RandRange(0, MapY - 1);
		int32 RandZ = FMath::RandRange(5, 10);
		RangeIncrementBy1(RandX, RandY, RandZ);
	}
}

void ATerrainGenerator::RangeIncrementBy1(int32 x, int32 y, int32 r)
{
	int32 Xs = x - r, Xe = x + r, Ys = y - r, Ye = y + r;

	if (Xs < 0) Xs = 0; 
	if (Xe >= MapX) Xe = MapX - 1;
	if (Ys < 0) Ys = 0;
	if (Ye >= MapY) Ye = MapY - 1;

	for (int32 a = Xs; a < Xe; a++)
	{
		for (int32 b = Ys; b < Ye; b++)
		{
			// 최대 높이 넘으면 정상화
			if (++HeightMap[a][b] >= MapZ) HeightMap[a][b] = MapZ - 1;
		}
	}
}

void ATerrainGenerator::SpawnStaticMeshByPath(const FSoftObjectPath& MeshPath, const FVector& Location, const FRotator& Rotation, const FVector& Scale, bool bCollision)
{
	// 경로가 유효한지 확인
    if (MeshPath.IsValid())
    {
        UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath.ToString());

        if (LoadedMesh)
        {
            // AStaticMeshActor 소환
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            
            // AStaticMeshActor 클래스를 사용하여 액터 소환
            AStaticMeshActor* MeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(
                AStaticMeshActor::StaticClass(), 
                Location, 
                Rotation,
                SpawnParams
            );

            if (MeshActor)
            {
                // 소환된 액터의 메시 컴포넌트에 로드된 메시 설정
                MeshActor->GetStaticMeshComponent()->SetStaticMesh(LoadedMesh);
				MeshActor->SetActorScale3D(Scale);
				MeshActor->SetActorEnableCollision(bCollision);
            }
        }
    }
}

void ATerrainGenerator::TeleportPlayerToLocation(FVector TargetLocation)
{
    // 1. 현재 월드를 가져옵니다.
    UWorld* World = GetWorld();
    if (!World) return;

    // 2. 현재 플레이어 컨트롤러를 가져옵니다.
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController) return;

    // 3. 컨트롤러가 빙의(Possess)하고 있는 폰/캐릭터를 가져옵니다.
    APawn* PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn) return;

    // 4. 순간이동 함수 호출
    PlayerPawn->TeleportTo(
        TargetLocation, 
        PlayerPawn->GetActorRotation(), // 현재 회전 유지
        false,                          // bIsATest: false (실제 이동)
        true                            // bNoCheck: true (충돌 검사 없이 강제 이동)
    );
}