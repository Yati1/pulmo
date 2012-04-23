#ifndef MOVABLE_SIGN_H
#define MOVABLE_SIGN_H

#include <Ogre.h>



class MovableSign
{
public:
	MovableSign(Ogre::SceneNode* pNode,Ogre::Quaternion qHidden,Ogre::Quaternion qShown,bool bShown=false);
	~MovableSign(void);

	void show()
	{
		m_bShown=true;
		m_animState=AS_SHOWING;
		m_bAnimating=true;
	}
	void hide()
	{
		m_bShown=false;
		m_animState=AS_HIDDING;
		m_bAnimating=true;
	}

	bool isShown(){return m_bShown; }

	bool isAnimating(){return m_bAnimating;}

	void update(float elapsedTime);
private:

	enum AnimState
	{
		AS_NONE,
		AS_HIDDING,
		AS_SHOWING
	};

	Ogre::SceneNode* m_pNode;
	Ogre::Quaternion m_qHidden;
	Ogre::Quaternion m_qShown;
	bool m_bShown;
	bool m_bAnimating;

	AnimState m_animState;
};
#endif

