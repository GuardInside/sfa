#include "src/core/log.h"
#include "src/core/event.h"
#include "src/core/app.h"

class CustomApp : public core::App
{
    enum Event
    {
        Timeout1 = 0x0001,
        Timeout2 = 0x0002,
        Timeout3 = 0x0004,
    };

    public:
        CustomApp()
        {
            setTimer(500000, Event::Timeout1);

            setTimer(1000000, Event::Timeout2);

            setTimer(5000000, Event::Timeout3, true);
        }

        void event(core::Event event) override
        {
            switch (event)
            {
                case Event::Timeout1:
                    
                    core::log("the time is out 1");

                    break;
                
                case Event::Timeout2:

                    core::log("the time is out 2");

                    break;

                case Event::Timeout3:

                    core::log("the time is out 3");

                    break;
            }
        }

        void serial(HardwareSerial& serial) override
        {
            char c = serial.read();

            switch (c)
            {
                case '0' + 1:
                    break;
                
                case '0' + 2:
                    break;
            }
        }

        void setup() override
        {
            // custom setup

            core::App::setup();
        }
} app;

run(app);

