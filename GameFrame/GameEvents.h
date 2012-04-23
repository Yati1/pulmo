#if !defined(GAMEEVENTS_H)
#define GAMEEVENTS_H

// Define game events here that we want to use in various un-related objects/classes/entities.
#define EVENT_ID_SHOOT "EVENT_ID_SHOOT"
#define EVENT_ID_GIVEHEALTH "EVENT_ID_GIVEHEALTH"

// Define custom event data related structs.
struct HealthEventData
{
	float fHealthInc;
};

#endif