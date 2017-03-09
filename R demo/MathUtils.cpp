#pragma once
#include"preinclude.h"
#include"MathUtils.h"

double MathUtils::RoundFloatTwoDigits(double source)
{
	return double(int(source * 100.0 + 0.5)) / 100.0;
}

int MathUtils::GenerateRandomInRange(int min, int max)
{
	if (min >= max)
	{
		return min;
	}
	int n = max - min + 1;
	if (n > RAND_MAX)
	{
		return (min + max) / 2;
	}
    int remainder = RAND_MAX % n;
    int x;
    do
    {
        x = rand();
    } while (x >= RAND_MAX - remainder);
	return min + x % n;
}