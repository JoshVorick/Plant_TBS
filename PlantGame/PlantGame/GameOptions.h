#pragma once

class GameOptions
{
private:
	bool turnLimit;	//Whether or not the game will have a turn-based end condition
	int maximumTurns;	//The maximum number of turns in the game, if applicable
	bool timeLimit;	//Whether or not the game will have a time-based end condition
	int maximumTime;	//The maximum amount of time the game is allowed to run for
	bool fogOfWar;	//Whether or not there will be fog of war for the players
					//Add more options as we think of them
public:
	GameOptions();	//I'm thinking we can avoid doing initializations and instead set everything
					//to a default from the .ini file and then allow the player to make
					//whatever changes they want, maybe allow saving new defaults
	~GameOptions();

	bool isTurnLimitOn() {return turnLimit;}	//Whether or not the turn-based end condition is on
	void flipTurnLimit() {turnLimit = !turnLimit;}	//Changes the state of turnLimit
	void setMaximumTurns(int maxTurns) {maximumTurns = maxTurns;}	//Sets the value of maximumTurns
	bool isTimeLimitOn() {return timeLimit;}	//Similar to above
	void flipTimeLimit() {timeLimit = !timeLimit;}
	void setMaximumTime(int maxTime) {maximumTime = maxTime;}	//Maybe we pass the value in seconds?
};
