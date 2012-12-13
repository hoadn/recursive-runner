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
#include "StateManager.h"

#include "base/PlacementHelper.h"
#include "base/TouchInputManager.h"
#include "systems/TransformationSystem.h"
#include "systems/RenderingSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/ADSRSystem.h"
#include "systems/TextRenderingSystem.h"
#include "systems/SessionSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/RunnerSystem.h"

#include "../RecursiveRunnerGame.h"

namespace Tutorial {
    enum Enum {
        IntroduceHero = 0,
        SmallJump,
        ScorePoints,
        BigJump,
        NewHero,
        MeetYourself,
        AvoidYourself,
        TheEnd
    };
}

struct TutorialEntities {
    Entity text;
    Entity anim;
};

struct TutorialStep {
    virtual void enter(SessionComponent* sc, TutorialEntities* entities) = 0;
    virtual bool mustUpdateGame(SessionComponent* sc, TutorialEntities* entities) = 0;
    virtual bool canExit(SessionComponent* sc, TutorialEntities* entities) = 0;
};

#include "tutorial/IntroduceHero.h"
#include "tutorial/SmallJump.h"
#include "tutorial/ScorePoints.h"
#include "tutorial/BigJump.h"
#include "tutorial/NewHero.h"
#include "tutorial/MeetYourself.h"
#include "tutorial/AvoidYourself.h"
#include "tutorial/TheEnd.h"

struct TutorialStateManager::TutorialStateManagerDatas {
    GameStateManager* gameStateMgr;
    float waitBeforeEnterExit;
    TutorialEntities entities;
    Entity titleGroup, title, hideText;
    Tutorial::Enum currentStep;
    std::map<Tutorial::Enum, TutorialStep*> step2mgr;
};

TutorialStateManager::TutorialStateManager(RecursiveRunnerGame* game) : StateManager(State::Tutorial, game) {
   datas = new TutorialStateManagerDatas;
   datas->gameStateMgr = new GameStateManager(game);
   datas->step2mgr[Tutorial::IntroduceHero] = new IntroduceHeroTutorialStep;
   datas->step2mgr[Tutorial::SmallJump] = new SmallJumpTutorialStep;
   datas->step2mgr[Tutorial::ScorePoints] = new ScorePointsTutorialStep;
   datas->step2mgr[Tutorial::BigJump] = new BigJumpTutorialStep;
   datas->step2mgr[Tutorial::NewHero] = new NewHeroTutorialStep;
   datas->step2mgr[Tutorial::MeetYourself] = new MeetYourselfTutorialStep;
   datas->step2mgr[Tutorial::AvoidYourself] = new AvoidYourselfTutorialStep;
   datas->step2mgr[Tutorial::TheEnd] = new TheEndTutorialStep;
}

TutorialStateManager::~TutorialStateManager() {
    delete datas->gameStateMgr;
    for(std::map<Tutorial::Enum, TutorialStep*>::iterator it=datas->step2mgr.begin();
        it!=datas->step2mgr.end();
        ++it) {
        delete it->second;
    }
    delete datas;
}

void TutorialStateManager::setup() {
    // setup game
    datas->gameStateMgr->setup();
    // setup tutorial
    Entity titleGroup = datas->titleGroup  = theEntityManager.CreateEntity();
    ADD_COMPONENT(titleGroup, Transformation);
    TRANSFORM(titleGroup)->z = 0.7;
    TRANSFORM(titleGroup)->rotation = 0.02;
    TRANSFORM(titleGroup)->parent = game->cameraEntity;
    ADD_COMPONENT(titleGroup, ADSR);
    ADSR(titleGroup)->idleValue = (PlacementHelper::ScreenHeight + PlacementHelper::GimpSizeToScreen(theRenderingSystem.getTextureSize("titre")).Y) * 0.6;
    ADSR(titleGroup)->sustainValue = (PlacementHelper::ScreenHeight - PlacementHelper::GimpSizeToScreen(theRenderingSystem.getTextureSize("titre")).Y) * 0.5
        + PlacementHelper::GimpHeightToScreen(10);
    ADSR(titleGroup)->attackValue = ADSR(titleGroup)->sustainValue - PlacementHelper::GimpHeightToScreen(5);
    ADSR(titleGroup)->attackTiming = 1;
    ADSR(titleGroup)->decayTiming = 0.1;
    ADSR(titleGroup)->releaseTiming = 0.3;
    TRANSFORM(titleGroup)->position = Vector2(0, ADSR(titleGroup)->idleValue);

    Entity title = datas->title = theEntityManager.CreateEntity();
    ADD_COMPONENT(title, Transformation);
    TRANSFORM(title)->size = PlacementHelper::GimpSizeToScreen(theRenderingSystem.getTextureSize("titre"));
    TRANSFORM(title)->parent = titleGroup;
    TRANSFORM(title)->position = Vector2::Zero;
    TRANSFORM(title)->z = 0.15;
    ADD_COMPONENT(title, Rendering);
    RENDERING(title)->texture = theRenderingSystem.loadTextureFile("titre");
    RENDERING(title)->cameraBitMask = 0x3;

    Entity hideText = datas->hideText = theEntityManager.CreateEntity();
    ADD_COMPONENT(hideText, Transformation);
    TRANSFORM(hideText)->size = PlacementHelper::GimpSizeToScreen(Vector2(776, 102));
    TRANSFORM(hideText)->parent = title;
    TRANSFORM(hideText)->position = (Vector2(-0.5, 0.5) + Vector2(41, -72) / theRenderingSystem.getTextureSize("titre")) * 
        TRANSFORM(title)->size + TRANSFORM(hideText)->size * Vector2(0.5, -0.5);
    TRANSFORM(hideText)->z = 0.01;
    ADD_COMPONENT(hideText, Rendering);
    RENDERING(hideText)->color = Color(130.0/255, 116.0/255, 117.0/255);
    RENDERING(hideText)->cameraBitMask = 0x3;
    
    Entity text = datas->entities.text = theEntityManager.CreateEntity();
    ADD_COMPONENT(text, Transformation);
    TRANSFORM(text)->size = TRANSFORM(hideText)->size;
    TRANSFORM(text)->parent = hideText;
    TRANSFORM(text)->z = 0.02;
    ADD_COMPONENT(text, TextRendering);
    TEXT_RENDERING(text)->charHeight = PlacementHelper::GimpHeightToScreen(60);
    TEXT_RENDERING(text)->hide = true;
    TEXT_RENDERING(text)->cameraBitMask = 0x3;
    TEXT_RENDERING(text)->color = Color(40.0 / 255, 32.0/255, 30.0/255, 0.8);
    TEXT_RENDERING(text)->positioning = TextRenderingComponent::CENTER;
    TEXT_RENDERING(text)->flags |= TextRenderingComponent::AdjustHeightToFillWidthBit;

    Entity anim = datas->entities.anim = theEntityManager.CreateEntity();
    ADD_COMPONENT(anim, Transformation);
    ADD_COMPONENT(anim, Rendering);
    RENDERING(anim)->cameraBitMask = 0x3;
    ADD_COMPONENT(anim, Animation);
}


///----------------------------------------------------------------------------//
///--------------------- ENTER SECTION ----------------------------------------//
///----------------------------------------------------------------------------//
void TutorialStateManager::willEnter(State::Enum from) {
    datas->gameStateMgr->willEnter(from);

    datas->waitBeforeEnterExit = TimeUtil::getTime();
    RENDERING(game->scorePanel)->hide = TEXT_RENDERING(game->scoreText)->hide = true;

    // hack lights/links
    SessionComponent* session = SESSION(theSessionSystem.RetrieveAllEntityWithComponent().front());
    std::for_each(session->coins.begin(), session->coins.end(), deleteEntityFunctor);
    session->coins.clear();
    std::for_each(session->links.begin(), session->links.end(), deleteEntityFunctor);
    session->links.clear();
    std::for_each(session->sparkling.begin(), session->sparkling.end(), deleteEntityFunctor);
    session->sparkling.clear();

    PlacementHelper::ScreenWidth = 60;
    PlacementHelper::GimpWidth = 3840;
    Vector2 c[] = {
        PlacementHelper::GimpPositionToScreen(Vector2(184, 568)),
        PlacementHelper::GimpPositionToScreen(Vector2(316, 448)),
        PlacementHelper::GimpPositionToScreen(Vector2(432, 612)),
        PlacementHelper::GimpPositionToScreen(Vector2(844, 600)),
        PlacementHelper::GimpPositionToScreen(Vector2(910, 450)),
        PlacementHelper::GimpPositionToScreen(Vector2(1046, 630)),
        PlacementHelper::GimpPositionToScreen(Vector2(1244, 564)),
        PlacementHelper::GimpPositionToScreen(Vector2(1340, 508)),
        PlacementHelper::GimpPositionToScreen(Vector2(1568, 460)),
        PlacementHelper::GimpPositionToScreen(Vector2(1820, 450)),
        PlacementHelper::GimpPositionToScreen(Vector2(1872, 580)),
        PlacementHelper::GimpPositionToScreen(Vector2(2096, 464)),
        PlacementHelper::GimpPositionToScreen(Vector2(2380, 584)),
        PlacementHelper::GimpPositionToScreen(Vector2(2540, 472)),
        PlacementHelper::GimpPositionToScreen(Vector2(2692, 572)),
        PlacementHelper::GimpPositionToScreen(Vector2(2916, 500)),
        PlacementHelper::GimpPositionToScreen(Vector2(3044, 568)),
        PlacementHelper::GimpPositionToScreen(Vector2(3560, 492)),
        PlacementHelper::GimpPositionToScreen(Vector2(3684, 566)),
        PlacementHelper::GimpPositionToScreen(Vector2(3776, 470))
    };
    std::vector<Vector2> coords;
    coords.resize(20);
    std::copy(c, &c[20], coords.begin());
    RecursiveRunnerGame::createCoins(coords, session, true);

    PlacementHelper::ScreenWidth = 20;
    PlacementHelper::GimpWidth = 1280;
}

bool TutorialStateManager::transitionCanEnter(State::Enum from) {
   bool gameCanEnter = datas->gameStateMgr->transitionCanEnter(from);

    if (TimeUtil::getTime() - datas->waitBeforeEnterExit < ADSR(datas->titleGroup)->attackTiming) {
        return false;
    }
    // TRANSFORM(datas->titleGroup)->position.X = theRenderingSystem.cameras[1].worldPosition.X;
    ADSR(datas->titleGroup)->active = true;
    RENDERING(datas->title)->hide = false;
    RENDERING(datas->hideText)->hide = false;

    ADSRComponent* adsr = ADSR(datas->titleGroup);
    TRANSFORM(datas->titleGroup)->position.Y = adsr->value;
    return (adsr->value == adsr->sustainValue) && gameCanEnter;
}

void TutorialStateManager::enter(State::Enum from) {
    SessionComponent* session = SESSION(theSessionSystem.RetrieveAllEntityWithComponent().front());
    session->userInputEnabled = false;

    datas->gameStateMgr->enter(from);
    datas->currentStep = Tutorial::IntroduceHero;
    datas->step2mgr[datas->currentStep]->enter(session, &datas->entities);
    RENDERING(game->scorePanel)->hide = TEXT_RENDERING(game->scoreText)->hide = true;
}


///----------------------------------------------------------------------------//
///--------------------- UPDATE SECTION ---------------------------------------//
///----------------------------------------------------------------------------//
void TutorialStateManager::backgroundUpdate(float) {
}

State::Enum TutorialStateManager::update(float dt) {
    SessionComponent* session = SESSION(theSessionSystem.RetrieveAllEntityWithComponent().front());

    if (datas->step2mgr[datas->currentStep]->mustUpdateGame(session, &datas->entities)) {
        datas->gameStateMgr->update(dt);
    }
    if (datas->step2mgr[datas->currentStep]->canExit(session, &datas->entities)) {
        if (datas->currentStep == Tutorial::TheEnd) {
            return State::Menu;
        } else {
            datas->currentStep = (Tutorial::Enum) (datas->currentStep + 1);
            datas->step2mgr[datas->currentStep]->enter(session, &datas->entities);
        }
    }
    return State::Tutorial;
}


///----------------------------------------------------------------------------//
///--------------------- EXIT SECTION -----------------------------------------//
///----------------------------------------------------------------------------//
void TutorialStateManager::willExit(State::Enum to) {
    ADSR(datas->titleGroup)->active = true;
    SessionComponent* session = SESSION(theSessionSystem.RetrieveAllEntityWithComponent().front());
    session->userInputEnabled = false;
    RENDERING(datas->title)->hide = true;
    RENDERING(datas->hideText)->hide = true;
    datas->gameStateMgr->willExit(to);
    RENDERING(game->scorePanel)->hide = TEXT_RENDERING(game->scoreText)->hide = false;
}

bool TutorialStateManager::transitionCanExit(State::Enum to) {
    TRANSFORM(datas->titleGroup)->position.Y = ADSR(datas->titleGroup)->value;
    return datas->gameStateMgr->transitionCanExit(to);
}

void TutorialStateManager::exit(State::Enum to) {
    datas->gameStateMgr->exit(to);
}

