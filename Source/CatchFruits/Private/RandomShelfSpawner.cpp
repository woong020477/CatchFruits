// RandomShelfSpawner.cpp
#include "RandomShelfSpawner.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARandomShelfSpawner::ARandomShelfSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트 필요 (동적 컴포넌트 부착용)
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;
}

void ARandomShelfSpawner::BeginPlay()
{
    Super::BeginPlay();
    SpawnBoxes();
}

void ARandomShelfSpawner::EnsureISMForMesh(UStaticMesh* Mesh)
{
    if (!Mesh || MeshToISM.Contains(Mesh)) return;

    UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>(this);
    ISM->SetStaticMesh(Mesh);
    ISM->SetCollisionProfileName(TEXT("BlockAll"));          // 필요에 따라 바꿔도 됨
    ISM->SetMobility(EComponentMobility::Static);
    ISM->NumCustomDataFloats = 0;                             // 커스텀 데이터 필요시 늘리기
    ISM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    if (OverrideMaterial)
    {
        // 모든 슬롯에 동일 머티리얼 적용 (필요 시 슬롯 인덱스 개별 지정)
        const int32 NumMats = Mesh->GetStaticMaterials().Num();
        for (int32 i = 0; i < NumMats; ++i)
        {
            ISM->SetMaterial(i, OverrideMaterial);
        }
    }

    ISM->RegisterComponent();

    MeshToISM.Add(Mesh, ISM);
}

void ARandomShelfSpawner::AddInstance(UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale)
{
    if (!Mesh) return;

    EnsureISMForMesh(Mesh);

    if (UInstancedStaticMeshComponent** Found = MeshToISM.Find(Mesh))
    {
        const FTransform T(Rotation, Location, Scale);
        (*Found)->AddInstance(T);
    }
}

void ARandomShelfSpawner::SpawnBoxes()
{
    if (SpawnableMeshes.Num() == 0) return;

    const FVector Origin = GetActorLocation();

    for (int32 Level = 0; Level < ShelfLevels; ++Level)
    {
        const FVector LevelBase = Origin + FVector(0.f, 0.f, Level * LevelOffset.Z);

        for (int32 i = 0; i < BoxesPerLevel; ++i)
        {
            // 각 층의 XY 범위 내 랜덤 위치
            const float X = FMath::FRandRange(-ShelfSize.X * 0.5f, ShelfSize.X * 0.5f);
            const float Y = FMath::FRandRange(-ShelfSize.Y * 0.5f, ShelfSize.Y * 0.5f);
            const FVector SpawnLoc = LevelBase + FVector(X, Y, 0.f);

            // Z축(월드 업축) 기준 랜덤 회전 (자연스러운 배치)
            const float Yaw = FMath::FRandRange(0.f, 360.f);
            const FRotator SpawnRot(0.f, Yaw, 0.f);

            // (옵션) 약간의 스케일 랜덤
            float S = FMath::FRandRange(UniformScaleRange.X, UniformScaleRange.Y);
            const FVector SpawnScale(S);

            // 랜덤 메쉬 선택
            const int32 RandomIndex = FMath::RandRange(0, SpawnableMeshes.Num() - 1);
            UStaticMesh* Mesh = SpawnableMeshes[RandomIndex];

            AddInstance(Mesh, SpawnLoc, SpawnRot, SpawnScale);
        }
    }
}

