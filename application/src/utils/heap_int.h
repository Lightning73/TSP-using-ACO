/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "../settings.h"

/// @brief Structure représentant un noeud dans le tas binaire
typedef struct HeapIntPair
{
    /// @brief La priorité du noeud
    float priority;
    /// @brief La valeur du noeud
    int value;
} HeapIntPair;


/// @brief Structure représentant un tas binaire
typedef struct HeapInt
{
    HeapIntPair* pairs;
    int size;
    int capacity;
} HeapInt;

