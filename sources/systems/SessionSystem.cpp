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
#include "SessionSystem.h"

INSTANCE_IMPL(SessionSystem);

SessionSystem::SessionSystem() : ComponentSystemImpl<SessionComponent>("Session") {
    SessionComponent tc;
    componentSerializer.add(new Property<int>(HASH("num_players", 0x0), OFFSET(numPlayers, tc)));
    componentSerializer.add(new EntityProperty(HASH("current_runner", 0x0), OFFSET(currentRunner, tc)));
    componentSerializer.add(new Property<bool>(HASH("user_input_enabled", 0x0), OFFSET(userInputEnabled, tc)));
    componentSerializer.add(new VectorProperty<Entity>(HASH("runners", 0x0), OFFSET(runners, tc)));
    componentSerializer.add(new VectorProperty<Entity>(HASH("coins", 0x0), OFFSET(coins, tc)));
    componentSerializer.add(new VectorProperty<Entity>(HASH("players", 0x0), OFFSET(players, tc)));
    componentSerializer.add(new VectorProperty<Entity>(HASH("links", 0x0), OFFSET(links, tc)));
    componentSerializer.add(new VectorProperty<Entity>(HASH("sparkling", 0x0), OFFSET(sparkling, tc)));
}

void SessionSystem::DoUpdate(float) {
    // nothing
}
