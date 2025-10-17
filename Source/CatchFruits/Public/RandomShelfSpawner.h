// RandomShelfSpawner.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "RandomShelfSpawner.generated.h"

UCLASS()
class CATCHFRUITS_API ARandomShelfSpawner : public AActor
{
    GENERATED_BODY()

public:
    ARandomShelfSpawner();

protected:
    virtual void BeginPlay() override;

public:
    // 스폰할 "메쉬"들 (상자 등)
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<UStaticMesh*> SpawnableMeshes;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    FVector ShelfSize = FVector(600.f, 200.f, 300.f); // 선반 크기 (X,Y,Z)

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 ShelfLevels = 3; // 3층

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 BoxesPerLevel = 4; // 한 층에 몇 개씩

    UPROPERTY(EditAnywhere, Category = "Spawn")
    FVector LevelOffset = FVector(0.f, 0.f, 120.f); // 층 높이 간격

    // (옵션) 모든 인스턴스에 적용할 오버라이드 머티리얼
    UPROPERTY(EditAnywhere, Category = "Spawn|Optional")
    UMaterialInterface* OverrideMaterial = nullptr;

    // (옵션) 인스턴스 스케일 범위
    UPROPERTY(EditAnywhere, Category = "Spawn|Optional")
    FVector2D UniformScaleRange = FVector2D(1.0f, 1.0f);

private:
    // 메쉬별 인스턴스 컴포넌트
    UPROPERTY(Transient)
    TMap<UStaticMesh*, UInstancedStaticMeshComponent*> MeshToISM;

    void EnsureISMForMesh(UStaticMesh* Mesh);
    void AddInstance(UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale);
    void SpawnBoxes();
};
