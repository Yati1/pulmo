#ifndef RANDOM_UTIL_H
#define RANDOM_UTIL_H

class RandomUtil
{
public:
	static float randomFloat(float fMax,float fMin)
	{
		float fRandom = (((float)rand()/(float)RAND_MAX) *(fMax-fMin)+1);

		if(fRandom>fMax)
			fRandom=fMax;

		return  fRandom;
	}

	static bool approximatelyEqualTo(float val1, float val2,float varience)
	{
		if(val1 <= val2+varience && val1 >= val2-varience)
			return true;
		else
			return false;
	}
};

#endif