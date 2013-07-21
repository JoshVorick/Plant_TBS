#pragma once

class EvoTree
{
private:
	double maxHPBonus;	//Stored as a decimal, holds percent increase of max hp for player	
	double mineralEfficiencyBonus;	//Same idea as above, this time for mineral usage, reducing costs for player
	double waterEfficiencyBonus;	//Same as mineralEfficiencyBonus, but with water
	double seedBonus;	//Percent increase in seeds yielded by player
	//Add more stuff as we come up with it

public:
	EvoTree();
	~EvoTree();
	double getMaxHPBonus() {return maxHPBonus;}	//Returns bonus to max hp
	double getMineralEfficiencyBonus() {return mineralEfficiencyBonus;}	//Returns bonus to mineral efficiency
	double getWaterEfficiencyBonus() {return waterEfficiencyBonus;}	//Returns bonus to water efficiency
	double getSeedBonus() {return seedBonus;}	//Returns bonus to seed production
};
