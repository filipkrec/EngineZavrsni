#pragma once 
#include <string>
#include <fstream>

std::string get_scores()
{
	std::string scores = "";

	unsigned int score;
	std::ifstream scoreFile("Scores.txt");
	int i = 1;

	while (scoreFile >> score)
	{
		scores += std::to_string(i) + " " + std::to_string(score) + '\n';
		++i;
		if (i > 10)
			break;
	}

	return scores;
}

void set_scores(unsigned int score)
{
	unsigned int s;
	unsigned int tempS = 0;
	std::string scores = "";
	std::ifstream scoreFileIn("Scores.txt");
	int i = 1;

	while (i < 10)
	{
		if (!(scoreFileIn >> s))
		{
			scores += std::to_string(tempS == 0 ? score : tempS) + '\n';
			break;
		}

		if (s > score)
		{
			scores += std::to_string(s) + '\n';
		}

		if (s < score)
		{
			if (tempS == 0)
			{
				scores += std::to_string(score) + '\n';
				tempS = s;
			}
			else
			{
				scores += std::to_string(tempS) + '\n';
				tempS = s;
			}
		}
		++i;
	}

	scoreFileIn.close();

	std::ofstream scoreFileOut("Scores.txt");
	scoreFileOut << scores;
	scoreFileOut.close();
}