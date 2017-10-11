#pragma once
#include "FBullCowGame.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>

#define TMap std::map //we're substituting TMap to conform to UnReal's coding standard

using int32 = int;
using FText = std::string;

FBullCowGame::FBullCowGame() {  }

//getters return our private members for use in areas outside the class (by calling our get functions)
int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddwnWordLength() const { return MyHiddenWord.length(); }

bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	//maps word length to max tries
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7,15}, {8,20}, {9,40} };
	return WordLengthToMaxTries[MyHiddenWord.length()]; //use square brackets to access elements
}

void FBullCowGame::ChooseDifficulty() //:: is a scope operator needed to access the class methods
{
	std::cout << "\n\nBefore you begin please select your difficulty: (enter 1, 2, 3, 4, or 5)\n\n";
	std::cout << "Word Novice: In this mode there are only 3 and 4 letter words\n";
	std::cout << "(enter 1 for this mode)\n\n";
	std::cout << "Word Pro: In this mode there are only 5 and 6 letter words\n";			
	std::cout << "(enter 2 for this mode)\n\n";
	std::cout << "Word Champion: In this mode there are only 7 and 8 letter words\n";
	std::cout << "(enter 3 for this mode)\n\n";
	std::cout << "Word King: This mode is a huge step up from the rest with 9 to 16 letter words!\n";
	std::cout << "(enter 4 for this mode)\n\n";
	std::cout << "Word God: This mode is for the true word master with words ranging from 3-16 letters long!\n";
	std::cout << "(enter 5 for this mode)\n\n";
	std::cout << "NOTE: The number of tries you get varies depending on the difficulty\n\n"; 
	std::cin >> Difficulty;
	/*without this it will count our selection as a guess so basically we have 5 difficulties and it 
	will take out that many characters until it reachs a newline then it stops*/
	std::cin.ignore(5, '\n');
	switch (Difficulty)
	{
	case 1:
		Easy();
		break;
	case 2:
		Medium();
		break;
	case 3:
		Hard();
		break;
	case 4:
		Harder();
		break;
	case 5:
		Hardest();
		break;
	default:
		std::cerr << "Oops that's not good :/\n";
		exit(EXIT_FAILURE);
	}
  
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	
	if (!IsIsogram(Guess)) //if the guess isn't an isogram 
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowerCase(Guess)) //if the guess isn't all lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	if (Guess.length() != GetHiddwnWordLength()) //if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

//recieves a VALID guess, increments turn, and returns count
//this function also creates the ability to display bulls and cows
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount; 
	int32 WordLength = MyHiddenWord.length(); //assuming same length as guess

	//loop through all letters that are in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		//compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			//if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) 
			{
				if (GChar == MHWChar) //if they're in the same place
				{
					BullCowCount.Bulls++; //increment bulls 
				}
				else
				{
					BullCowCount.Cows++; //must be a cow
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const //our parameter is Word becuase its the hidden word we chose
{
	//treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; //set up our map (this is the syntax)
	for (auto Letter : Word)  //for all letters in the word
	{
		Letter = tolower(Letter); //calls the tolower function so we can handle mixed cases 
								 //(we want are next loop to only deal with lowercase letters)
		//if the letter is in the map
			if (LetterSeen[Letter]) //LetterSeen is a list of all the letters that have been seen 
			{					    //and [Letter] is the check for seen letters
				return false; //we do NOT have an isogram
			}
			else 
			{
				LetterSeen[Letter] = true; //add the letter to the map as seen
			}
	}
	
	return true; //for example in cases where /0 is entered
}

bool FBullCowGame::IsLowerCase(FString Word) const
{
	if (Word.length() <= 1) { return true;  }
	
	TMap <char, bool> LowerLetter;
	//for all letters in the word
	for (auto Letter : Word)
	{
		//if any or all of the letters are NOT lowercase
		if (!islower(Letter))
		{
			return false;
		}
	}

	return true;
}

void FBullCowGame::Easy()
{
	std::vector<FString> RandomWord;
	FString Word = "";

	std::ifstream PossibleWords;
	PossibleWords.open("easy.txt");

	//this is an error check if the file doesn't open we'll know about it!
	//this should always be present when file streaming forgetting this could get very confusing...
	if (PossibleWords.fail())
	{
		std::cerr << "The file failed to open :(\n";
		exit(EXIT_FAILURE);
	}

	/*getline extracts characters from a stream in this case possible_words.txt)
	which is a file and thus is apart of fstream
	and stores them in a string (in this case Word)*/
	while (std::getline(PossibleWords, Word))
	{
		//we then add the characters we stored in our string to our vector! 
		RandomWord.push_back(Word);
	}

	//closes our file you don't really need this for it to work but it is a good practice to always close files
	PossibleWords.close();

	srand((unsigned int)time(NULL));

	FString HIDDEN_WORD = RandomWord[rand() % RandomWord.size()];
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;
	
	return;
}

//I did the exact same thing in the functions below so I didn't feel the need to comment what anything does

void FBullCowGame::Medium()
{
	std::vector<FString> RandomWord; 
	FString Word = "";

	std::ifstream PossibleWords;
	PossibleWords.open("medium.txt");

	if (PossibleWords.fail())
	{
		std::cerr << "The file failed to open :(\n";
		exit(EXIT_FAILURE);
	}

	while (std::getline(PossibleWords, Word))
	{ 
		RandomWord.push_back(Word);
	}

	PossibleWords.close();

	srand((unsigned int)time(NULL));

	FString HIDDEN_WORD = RandomWord[rand() % RandomWord.size()];
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}

void FBullCowGame::Hard()
{
	std::vector<FString> RandomWord;
	FString Word = "";

	std::ifstream PossibleWords;
	PossibleWords.open("hard.txt");

	if (PossibleWords.fail())
	{
		std::cerr << "The file failed to open :(\n";
		exit(EXIT_FAILURE);
	}

	while (std::getline(PossibleWords, Word))
	{
		RandomWord.push_back(Word);
	}

	PossibleWords.close();

	srand((unsigned int)time(NULL));

	FString HIDDEN_WORD = RandomWord[rand() % RandomWord.size()];
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}

void FBullCowGame::Harder()
{
	std::vector<FString> RandomWord;
	FString Word = "";

	std::ifstream PossibleWords;
	PossibleWords.open("harder.txt");

	if (PossibleWords.fail())
	{
		std::cerr << "The file failed to open :(\n";
		exit(EXIT_FAILURE);
	}

	while (std::getline(PossibleWords, Word))
	{ 
		RandomWord.push_back(Word);
	}

	PossibleWords.close();

	srand((unsigned int)time(NULL));

	FString HIDDEN_WORD = RandomWord[rand() % RandomWord.size()];
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}

void FBullCowGame::Hardest()
{
	std::vector<FString> RandomWord;
	FString Word = "";

	std::ifstream PossibleWords;
	PossibleWords.open("hardest.txt");

	if (PossibleWords.fail())
	{
		std::cerr << "The file failed to open :(\n";
		exit(EXIT_FAILURE);
	}

	while (std::getline(PossibleWords, Word))
	{
		RandomWord.push_back(Word);
	}

	PossibleWords.close();

	srand((unsigned int)time(NULL));

	FString HIDDEN_WORD = RandomWord[rand() % RandomWord.size()];
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}
