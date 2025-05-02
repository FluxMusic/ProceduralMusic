// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Forward Declarations
struct FRandomStream;

class DnBIntroGenerator
{
public:
    DnBIntroGenerator(FRandomStream& SeedIn);

private:
    FRandomStream& Seed;
};