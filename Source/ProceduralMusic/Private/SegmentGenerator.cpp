// Fill out your copyright notice in the Description page of Project Settings.


#include "SegmentGenerator.h"

#include "Math/RandomStream.h"

SegmentGenerator::SegmentGenerator(FRandomStream& SeedIn, UDataTable* ScalesIn)
: Seed(SeedIn)
, Scales(ScalesIn)
{}