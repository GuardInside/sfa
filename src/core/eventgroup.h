#include "event.h"

namespace core
{
    class EventGroup
    {
        public:
            EventGroup();

            void setEvent(Event event);
            
            void unsetEvent(Event event);

            bool hasEvent(Event event) const;

            bool hasEvents() const;

            Event nextEvent() const;

        private:
            volatile char events;
    };
}