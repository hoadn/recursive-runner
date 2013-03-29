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
#include <glm/glm.hpp>

struct CameraTargetComponent {
    CameraTargetComponent() : camera(0), enabled(false), cameraSpeed(0.0f) {}
    Entity camera;
    glm::vec2 offset;
    float maxCameraSpeed;
    bool enabled;
    glm::vec2 cameraSpeed;
};

#define theCameraTargetSystem CameraTargetSystem::GetInstance()
#define CAM_TARGET(e) theCameraTargetSystem.Get(e)

UPDATABLE_SYSTEM(CameraTarget)
};
