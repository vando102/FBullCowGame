#ifndef __FBULL_COW_GAME_H__
#define __FBULL_COW_GAME_H__

#include <string>

//to make syntax conform to Unreal
using FString = std::string;
using int32 = int;

//all values iniatlized to 0
struct FBullCowCount //best to keep structs simple
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};

class FBullCowGame 
{
public: //we create methods to tell the compiler what we want this class to do
	FBullCowGame(); //constructor
	
	//difficulty functions
	void Easy();
	void Medium();
	void Hard();
	void Harder();
	void Hardest();

	int32 GetMaxTries() const; //returns max tries
	int32 GetCurrentTry() const; //returns current try
	int32 GetHiddwnWordLength() const; // returns hidden word length
	bool IsGameWon() const;
	EGuessStatus CheckGuessValidity(FString) const; 

	void ChooseDifficulty();
	FBullCowCount SubmitValidGuess(FString);
	//TODO provide a way to pick the same word again 
	//TODO add a timer (so people can't cheat)

private: //variables are generally private, and methods or functions are generally public
	//see the difficulty member functions for initialisation
	int32 MyCurrentTry;
	FString  MyHiddenWord;
	int32 Difficulty;
	bool bGameIsWon;

	bool IsIsogram(FString) const;
	bool IsLowerCase(FString) const;
};

#endif