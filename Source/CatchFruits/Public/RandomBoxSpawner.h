#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomBoxSpawner.generated.h"

UCLASS()
class CATCHFRUITS_API ARandomBoxSpawner : public AActor
{
    GENERATED_BODY()

public:
    ARandomBoxSpawner();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

public:
    //스폰할 메쉬들
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    TArray<UStaticMesh*> SpawnableMeshes;

    //X축 박스 개수
    UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (ClampMin = "1"))
    int32 BoxCountX = 5;

    //Z 층 수
    UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (ClampMin = "1"))
    int32 BoxCountZ = 3;

    // X축 간격 
    UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (ClampMin = "0.0"))
    float BoxSpacingX = 200.f;

    // Z축 간격 
    UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (ClampMin = "0.0"))
    float BoxSpacingZ = 150.f;

    // 박스 크기
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    FVector BoxSize = FVector(100.f, 100.f, 100.f);

    // 회전 랜덤
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    bool bRandomRotation = true;

    //게임 시작 시 랜덤 개수 스폰
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    bool bRandomCountOnBeginPlay = true;

    //최소 스폰 수
    UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (ClampMin = "1"))
    int32 MinSpawnCount = 4;

private:
    void ClearSpawnedMeshes();
    void SpawnBoxes(bool bRuntimeRandom);

    UPROPERTY()
    TArray<UStaticMeshComponent*> SpawnedComponents;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneRoot;
};
