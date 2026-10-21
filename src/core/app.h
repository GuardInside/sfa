#include "eventgroup.h"

namespace core
{
    struct Timer
    {
        Timer()
            : timestamp(-1)
            , timeout(-1)
            , event(-1)
            , singleShoot(0)
        {
        }

        bool isValid() const
        {
            return timeout != -1 && event != -1;
        }

        void invalidate()
        {
            timestamp = -1;
            timeout   = -1;
            event     = -1;
        }

        unsigned long timestamp;
        unsigned long timeout; 
        Event         event;
        bool          singleShoot;
    };

    class App
    {
        public:
            // Process events
            //
            // 
            virtual void event(Event event)
            {
                ;
            }

            // Process serial
            virtual void serial(HardwareSerial& serial)
            {
                ;
            }

            // Set new timer
            bool setTimer(unsigned long timeout, Event event, bool singleShoot = false)
            {
                for (Timer & timer: _timers)
                {
                    if (!timer.isValid())
                    {
                        timer.timeout     = timeout;
                        timer.event       = event;
                        timer.singleShoot = singleShoot;

                        return true;
                    }
                }

                return false;
            }

            void unsetTimer(Event event)
            {
                for (Timer & timer: _timers)
                {
                    if (timer.event == event)
                    {
                        timer.invalidate();
                    }
                }
            }

            virtual void setup()
            {
                Serial.begin(9600);
            }

            void loop()
            {
                // timers

                const unsigned long timestamp = micros();

                for (Timer & timer: _timers)
                {
                    if (timer.isValid())
                    {
                        if (timestamp - timer.timestamp >= timer.timeout)
                        {
                            _eventGroup.setEvent(timer.event);

                            if (timer.singleShoot)
                            {
                                timer.invalidate();
                            }
                            else
                            {
                                timer.timestamp += timer.timeout;
                            }
                        }
                    }
                }

                // event loop

                while (_eventGroup.hasEvents())
                {
                    Event e = _eventGroup.nextEvent();

                    event(e);

                    _eventGroup.unsetEvent(e);
                }

                // serial loop
                
                if (Serial.available() > 0)
                {
                    serial(Serial);
                }
            }

            void setEvent(Event event)
            {
                _eventGroup.setEvent(event);
            }

        private:
            Timer      _timers[3];
            EventGroup _eventGroup;
    };
}

#define run(app) \
                 \
void setup()     \
{                \
    app.setup(); \
}                \
                 \
void loop()      \
{                \
    app.loop();  \
}