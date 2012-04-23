#include "MovableSign.h"


MovableSign::MovableSign(Ogre::SceneNode* pNode,Ogre::Quaternion qHidden,Ogre::Quaternion qShown,bool bShown)
{
	m_pNode=pNode;
	m_qHidden=qHidden;
	m_qShown=qShown;
	m_bShown=bShown;
	m_bAnimating=false;
	m_animState=AS_NONE;
}


MovableSign::~MovableSign(void)
{
	m_pNode=0;
}

void MovableSign::update(float elapsedTime)
{
	if(m_bAnimating)
	{
		float speed=1.5f;
		if(m_animState==AS_HIDDING)
		{

			if(m_pNode->getOrientation()!=m_qHidden)
			{
			
				Ogre::Quaternion q=Ogre::Quaternion::Slerp(speed*elapsedTime,m_pNode->getOrientation(),m_qHidden,false);
				m_pNode->setOrientation(q);
			
			}else							//if we're home
			{
				m_bAnimating=false;
				m_animState=AS_NONE;

			}
		}else
		if(m_animState==AS_SHOWING)
		{
			if(m_pNode->getOrientation()!=m_qShown)
			{
			
				Ogre::Quaternion q=Ogre::Quaternion::Slerp(speed*elapsedTime,m_pNode->getOrientation(),m_qShown,false);
				m_pNode->setOrientation(q);
			
			}else							//if we're home
			{
				m_bAnimating=false;
				m_animState=AS_NONE;

			}
		}
	}
}
