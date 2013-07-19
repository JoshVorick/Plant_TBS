#pragma once

#include "GameOptions.h"

GameOptions::GameOptions()
{
}

GameOptions::~GameOptions()
{
}

bool GameOptions::isTurnLimitOn()
{
	return turnLimit;
}

void GameOptions::flipTurnLimit()
{
	turnLimit = !turnLimit;
}

void GameOptions::setMaximumTurns(int maxTurns)
{
	maximumTurns = maxTurns;
}

bool GameOptions::isTimeLimitOn()
{
	return timeLimit;
}

void GameOptions::flipTimeLimit()
{
	timeLimit = !timeLimit;
}

void GameOptions::setMaximumTime(int maxTime)
{
	maximumTime = maxTime;
}