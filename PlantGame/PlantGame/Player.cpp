#pragma once
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::setName(std::string name)
{
	playerName = name;
}

std::string Player::getName()
{
	return playerName;
}

void Player::addToScore(int points)
{
	score += points;
}

int Player::getScore()
{
	return score;
}

EvoTree* Player::getEvoTree()
{
	return evoTree;
}