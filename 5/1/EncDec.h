//
// Created by vlad on 01.12.2019.
//
#pragma once
#include "Stream.h"
// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);