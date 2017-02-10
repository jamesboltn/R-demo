#pragma once
namespace MathUtils
{
	double RoundFloatTwoDigits(double source);
	int GenerateRandomInRange(int min, int max);//this function assumes min >=0 and max > min and max-min+1 > RAND_MAX
}
