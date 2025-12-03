#include "RandomBoxSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARandomBoxSpawner::ARandomBoxSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneRoot;
}

void ARandomBoxSpawner::OnConstruction(const FTransform& Transform)
{
    ClearSpawnedMeshes();
    SpawnBoxes(false);
}

void ARandomBoxSpawner::BeginPlay()
{
    ClearSpawnedMeshes();
    SpawnBoxes(true);
}

void ARandomBoxSpawner::ClearSpawnedMeshes()
{
    for (UStaticMeshComponent* Comp : SpawnedComponents)
    {
        if (Comp)
            Comp->DestroyComponent();
    }
    SpawnedComponents.Empty();
}

void ARandomBoxSpawner::SpawnBoxes(bool bRuntimeRandom)
{
    if (SpawnableMeshes.Num() == 0)
        return;

    int32 TotalSlots = BoxCountX * BoxCountZ;
    int32 SpawnCount = TotalSlots;

    if (bRuntimeRandom && bRandomCountOnBeginPlay)
    {
        int32 MaxPossible = TotalSlots;
        int32 MinPossible = FMath::Clamp(MinSpawnCount, 1, MaxPossible);
        SpawnCount = FMath::RandRange(MinPossible, MaxPossible);
    }

    TArray<int32> SlotIndices;
    for (int32 i = 0; i < TotalSlots; ++i)
        SlotIndices.Add(i);

    if (bRuntimeRandom)
        SlotIndices.Sort([](int32 A, int32 B) { return FMath::RandBool(); }); // 랜덤 셔플

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        int32 Index = SlotIndices[i];
        int32 z = Index / BoxCountX;
        int32 x = Index % BoxCountX;

        UStaticMesh* SelectedMesh = SpawnableMeshes[FMath::RandRange(0, SpawnableMeshes.Num() - 1)];
        if (!SelectedMesh)
            continue;

        FString CompName = FString::Printf(TEXT("Box_%d_%d"), z, x);
        UStaticMeshComponent* NewComp = NewObject<UStaticMeshComponent>(this, *CompName);
        NewComp->RegisterComponent();
        NewComp->SetStaticMesh(SelectedMesh);
        NewComp->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

        FVector RelativeLocation = FVector(
            x * (BoxSize.X + BoxSpacingX),
            0.f,
            z * (BoxSize.Z + BoxSpacingZ)
        );

        FRotator Rotation = FRotator::ZeroRotator;
        if (bRandomRotation)
        {
            Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
        }

        //  적용
        NewComp->SetRelativeLocationAndRotation(RelativeLocation, Rotation);
        SpawnedComponents.Add(NewComp);
    }
}
