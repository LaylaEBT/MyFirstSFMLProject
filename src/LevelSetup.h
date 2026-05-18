#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <slot.h>
#include <connection.h>

void setupLevel(int level, std::vector<Slot>& slots, std::vector<Connection>& connections, int k_cols, float k_spacing, float k_radius);