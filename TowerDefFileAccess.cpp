#include "TowerDefFileAccess.h"
#include <iostream>
#include <fstream>

void SaveGame(SaveFormat argSaveFormat,std::string argFileName)
{
	std::ofstream saveFile(argFileName);
	saveFile << argSaveFormat.MatrixX << " " << argSaveFormat.MatrixY << " " << argSaveFormat.PlayerHealth << " " << argSaveFormat.PlayerResource << " " << argSaveFormat.WaveCount << " " << argSaveFormat.Difficulty << " " << argSaveFormat.ElapsedTime << std::endl;
	for (int i = 0; i < argSaveFormat.MatrixRep.size(); i++)
	{
		for (int j = 0; j < argSaveFormat.MatrixRep[i].size(); j++)
		{
			if (argSaveFormat.MatrixRep[i][j].size() > 1)
			{
				for (int k = 0; k < argSaveFormat.MatrixRep[i][j].size(); k++)
				{
					if (k == argSaveFormat.MatrixRep[i][j].size()-1)
					{
						saveFile << argSaveFormat.MatrixRep[i][j][k];
					}
					else
					{
						saveFile << argSaveFormat.MatrixRep[i][j][k] << " ";
					}
				}
			}
			else
			{
				saveFile << argSaveFormat.MatrixRep[i][j][0];
			}
			saveFile << std::endl;
		}
	}
	saveFile.close();
}

SaveFormat LoadGame(const std::string argFileName)
{
	struct SaveFormat ReturnFormat;

	std::ifstream LoadFile(argFileName);
	LoadFile >> ReturnFormat.MatrixX >> ReturnFormat.MatrixY >> ReturnFormat.PlayerHealth >> ReturnFormat.PlayerResource >> ReturnFormat.WaveCount >> ReturnFormat.Difficulty >> ReturnFormat.ElapsedTime;
	ReturnFormat.MatrixRep.resize(ReturnFormat.MatrixX);
	for (int i = 0; i < ReturnFormat.MatrixX; i++)
	{
		ReturnFormat.MatrixRep[i].resize(ReturnFormat.MatrixY);
	}
	std::string fileline;
	std::getline(LoadFile, fileline);//Here because of the >> operator
	for (int i = 0; i < ReturnFormat.MatrixX; i++)
	{
		for (int j = 0; j < ReturnFormat.MatrixY; j++)
		{
			std::getline(LoadFile, fileline);
			ReturnFormat.MatrixRep[i][j].push_back(fileline);
		}
	}
	return ReturnFormat;
}
