/*This is the console exectuable, that makes use of the BullCow class 
This acts as a view in an MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/

//TODO add pre/post  conditions to all my members/functions
//TODO add my personal info (such as name) in my .h file
#pragma once
#include "FBullCowGame.h"
#include <iostream>
#include <string>


using std::cout;
using FText = std::string; //FText in unreal is used for interaction with the user
using int32 = int; //we're going to need to get use to unreal's coding standards

//remember to use std:: because we are not using namespace!!!

void PrintIntro(); // function declaration
void PlayGame();
FText GetValidGuess(); 
bool AskToPlayAgain();
void PrintGameSummary();

//create an object (BCGame) of type FBullCowGame
FBullCowGame BCGame; //an instance of a new game

//the entry point for are application
int main() //since we added functions main is much easier to understand
{
	do
	{
		BCGame.ChooseDifficulty();
		PrintIntro();
		PlayGame();
	} while (AskToPlayAgain() == true);
	return 0; //exit the application
}

//introduce the game
void PrintIntro() //function definition
{
	cout << "\n\nWelcome to Bulls and Cows, a fun word game!\n";
	cout << '\n';
	cout << "           }   {            ___" << '\n';
	cout << "           (o o)           (o o)" << '\n';
	cout << "    /-------\\ /             \\ /------\\ " << '\n';
	cout << "   / | BULL |O               O| COW | \\ " << '\n';
	cout << " *   |-,--- |                 |-----|  * " << '\n';
	cout << "     ^      ^                 ^     ^ " << '\n';
	cout << "Can you guess the " << BCGame.GetHiddwnWordLength() << " letter isogram I'm thinking of?\n";
	cout << '\n';
	return;
}

void PlayGame()
{
	int32 MaxTries = BCGame.GetMaxTries();

	//loop asking for guesses while the game is NOT won
	//and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess(); 

		//submit valid guess to the game
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		cout << "Bulls = " << BullCowCount.Bulls;
		cout << " Cows = " << BullCowCount.Cows << "\n\n";
	}
	
	PrintGameSummary();
}

//loop continually until the player gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do
	{
		//get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();

		cout << "Try " << CurrentTry << " of "<< BCGame.GetMaxTries() << " Enter your guess: ";
		std::getline(std::cin, Guess);
		//check if the users guess is valid or not
	    Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:			
			cout << "Please enter a " << BCGame.GetHiddwnWordLength() << " letter word.\n\n";
			break;

		case EGuessStatus::Not_Isogram:
			cout << "Please enter an isogram (a word with no repeating letters).\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			cout << "Please enter a word with only lowercase letters!\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); //keep looping until we get no errors
	
	return Guess;
}

//asks the player if he/she wants to play again
//returns true if yes, false if anything else
bool AskToPlayAgain() //bools can only return true or false
{
	cout << "Do you want to play again? (y/n) \n";
	FText Response = "";
	std::getline(std::cin, Response);

	return (Response[0] == 'y' || (Response[0] == 'Y')); //must return to main
															 //because that's where all the magic happens
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		cout << "WELL DONE - YOU WIN!!\n\n";
	}
	else
	{
		cout << "Sorry you lost :( \n" << " \nBetter luck next time xP\n\n";
	}
}
