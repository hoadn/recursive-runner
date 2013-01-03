/*
    This file is part of RecursiveRunner.

    @author Soupe au Caillou - Pierre-Eric Pelloux-Prayer
    @author Soupe au Caillou - Gautier Pelloux-Prayer

    RecursiveRunner is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3.

    RecursiveRunner is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RecursiveRunner.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "systems/System.h"
#include "base/Vector2.h"

struct PlatformerComponent {
    PlatformerComponent() : previousPosition(Vector2::Zero), onPlatform(0) {}
    Vector2 previousPosition, offset;
    Entity onPlatform;
    std::map<Entity, bool> platforms;
};

#define thePlatformerSystem PlatformerSystem::GetInstance()
#define PLATFORMER(e) thePlatformerSystem.Get(e)

UPDATABLE_SYSTEM(Platformer)
};