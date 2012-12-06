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
#include "PlayerSystem.h"
#include "RunnerSystem.h"

INSTANCE_IMPL(PlayerSystem);

PlayerSystem::PlayerSystem() : ComponentSystemImpl<PlayerComponent>("Player") {
    PlayerComponent tc;
    componentSerializer.add(new Property(OFFSET(score, tc), sizeof(int)));
    componentSerializer.add(new Property(OFFSET(runnersCount, tc), sizeof(int)));
    componentSerializer.add(new Property(OFFSET(ready, tc), sizeof(bool)));
}

void PlayerSystem::DoUpdate(float dt __attribute__((unused))) {

}

