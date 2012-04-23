
#ifndef _SIMPLETON_HEADER_
#define _SIMPLETON_HEADER_

/*
	Simpleton - the simple singleton

	GetSingleton checks whether the s_singleton is initialized then acts accordingly 
*/

template < class T >
class Simpleton
{
public:

	static void DestroySingleton()
	{
		delete s_singleton;
		s_singleton = 0;
	}

	static bool SingletonExists()
	{
		return (s_singleton != 0);
	}

	static T * GetSingleton()
	{
		if(s_singleton!=0)
			return s_singleton;
		else{
			CreateSingleton();
			return s_singleton;
		}
	}

protected:
	virtual ~Simpleton() {};

	static void CreateSingleton()
	{
		s_singleton = new T;
	}

	static T * s_singleton;
};



#endif
