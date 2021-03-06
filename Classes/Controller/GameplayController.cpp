﻿#include "GameplayController.h"
#include "GameplayView.h"
#include "SimpleAudioEngine.h"
#include "GameplayModel.h"
#include "Hero.h"
#include "Defined.h"
#include "BearData.h"

USING_NS_CC;
GameplayController::GameplayController(void)
{
	_view = NULL;

	_beginFlag = false;
	_pauseFlag = false;
}


GameplayController::~GameplayController(void)
{
	
}

bool GameplayController::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());

		_view = GameplayView::create(this);
		CC_BREAK_IF(!_view);
		addChild(_view);
		
		scheduleUpdate();
		bRet = true;
	} while (0);
	return bRet;
}

void GameplayController::update( float dt )
{
	if (!_beginFlag)
	{
		_beginFlag = true;
		GameplayModel::sharedModel()->getHero()->setAwake(true);
		_view->showGameView();
		//pause();
	}

	if(_pauseFlag) return;
	if (GameplayModel::sharedModel()->isGameOver())
	{
		pause();
		_view->showResult();
	}
	else
	{
		GameplayModel::sharedModel()->update(dt);

		_view->update(dt);
	}
}

void GameplayController::didPause()
{
	pause();
	_view->showPauseUI();
}

void GameplayController::didResume()
{
	resume();
	_view->showGameView();
}

void GameplayController::pause()
{
	_pauseFlag = true;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound_button.mp3");
	operateAllSchedulerAndActions(_view, k_Operate_Pause);
}

void GameplayController::resume()
{
	_pauseFlag = false;
	operateAllSchedulerAndActions(_view, k_Operate_Resume);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}


void GameplayController::operateAllSchedulerAndActions( cocos2d::CCNode* node, OperateFlag flag )
{
	if(node->isRunning()){
		switch (flag) {
		case k_Operate_Pause:
			node->pauseSchedulerAndActions();
			break;
		case k_Operate_Resume:
			node->resumeSchedulerAndActions();
			break;
		default:
			break;
		}
		CCArray* array = node->getChildren();
		if(array != NULL && array->count()>0){
			CCObject* iterator;
			CCARRAY_FOREACH(array, iterator){
				CCNode* child = (CCNode*)iterator;
				operateAllSchedulerAndActions(child, flag);
			}
		}
	}
}

void GameplayController::didTouchBegan()
{
	Hero* hero = GameplayModel::sharedModel()->getHero();
	if (GameplayModel::sharedModel()->isHeroOnTheGround())
	{
		hero->jump();
		GameplayModel::sharedModel()->setTapDown(true);
	}
	else
	{
		GameplayModel::sharedModel()->setTapDown(true);
	}
}

void GameplayController::didTouchEnded()
{
	GameplayModel::sharedModel()->setTapDown(false);
}

void GameplayController::didTouchCancelled()
{
	GameplayModel::sharedModel()->setTapDown(false);
}

void GameplayController::didReset()
{
	GameplayModel::sharedModel()->getHero()->setState(Hero::kHeroState_normal);
	BearData::sharedData()->reset();
	didResume();
}
