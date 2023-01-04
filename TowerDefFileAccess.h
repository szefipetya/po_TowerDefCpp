#pragma once
#include <string>
#include <vector>
//This is the format which is used to save or load a gamestate a.k.a. the necessary information for a game to be save and loaded
struct SaveFormat
{
	int MatrixX;
	int MatrixY;
	int PlayerHealth;
	int PlayerResource;
	int WaveCount;
	int ElapsedTime; //std::chrono::milliseconds duration | formából a duration.count() -al elvileg meglehet kapni az int-et.....
	int Difficulty;
	std::vector<std::vector<std::vector<std::string>>> MatrixRep; //Representation of the matrix in strings
};
//Saves the information stored in the given SaveFormat to save the gamestate to a filename given as a parameter
void SaveGame(SaveFormat argSaveFormat, const std::string argFileName);
//Returns a SaveFormat from a file given as an argument
SaveFormat LoadGame(const std::string argFileName);



