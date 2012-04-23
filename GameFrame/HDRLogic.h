#ifndef HDR_LOGIC_H
#define HDR_LOGIC_H

#include "ListenerFactoryLogic.h"

//The compositor logic for the hdr compositor
class HDRLogic : public ListenerFactoryLogic
{
protected:
	/** @copydoc ListenerFactoryLogic::createListener */
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
};

#endif