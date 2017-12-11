#ifndef Event_h
#define Event_h

#include "Parabola.h"

class Event
{
public:
	Point*		point;  //point of the event	(top circle point for circle event, focus point for place event)
	bool		place_event;  //is the event is place event
	double		y;  //coordinate of point
	Parabola	*arch;   //if (place_event) new arch begins 

	Event(Point * a, bool isP_event)
	{
		point = a;
		place_event = isP_event;
		y = a->y;
		arch = 0;
	}

	struct CompareEvent : public std::binary_function<Event*, Event*, bool>
	{
		bool operator()(const Event* l, const Event* r) const { return (l->y < r->y); }
	};
};

#endif
