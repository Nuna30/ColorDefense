// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"

FChunk::FChunk()
{
	this->Property = EChunkProperty::Empty;
	this->Index = FIntVector::ZeroValue;
}

FChunk::FChunk(const FIntVector& InChunkIndex, EChunkProperty InProperty)
{
	this->Property = InProperty;
	this->Index = InChunkIndex;
}