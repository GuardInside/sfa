#include "eventgroup.h"

namespace core
{
    EventGroup::EventGroup()
        : events(0x0000)
    {

    }

    void EventGroup::setEvent(Event event)
    {
        events |= event;
    }

    void EventGroup::unsetEvent(Event event)
    {
        events &= ~event;
    }

    bool EventGroup::hasEvent(Event event) const
    {
        return events & event;
    }

    bool EventGroup::hasEvents() const
    {
        return events != 0x0000;
    }

    Event EventGroup::nextEvent() const
    {
        for (int i = 0; i < 8 * sizeof(events); ++i)
        {
            if ((events >> i) & 0x1)
            {
                return 0x1 << i;
            }
        }

        return 0x0000;
    }
}