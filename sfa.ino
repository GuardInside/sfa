#include <GyverOLED.h>

#include "src/core/log.h"
#include "src/core/event.h"
#include "src/core/app.h"

enum Event
{
    Timeout1     = 0x0001,
    Timeout2     = 0x0002,
    Timeout3     = 0x0004,
    Paint        = 0x0008,
    BufferIsFull = 0x0010,
};

enum Pin
{
    adc = A0,
};

class CustomApp : public core::App
{
    public:
        CustomApp()
        {
            
        }

        void event(core::Event event) override
        {
            switch (event)
            {
                case Event::Timeout1:
                    readSample();

                    setTimer(100, Event::Timeout1, true);

                    break;
                
                case Event::Timeout2:

                    core::log("the time is out 2");

                    break;

                case Event::Timeout3:

                    core::log("the time is out 3");

                    break;

                case Event::BufferIsFull:
                    updateMetrics();
                    
                    break;

                case Event::Paint:
                    paint();

                    setTimer(30000, Event::Paint, true);

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

            analogReference(EXTERNAL);

            //

            _display.init();
            _display.clear();

            //

            setEvent(Event::Paint);

            setEvent(Event::Timeout1);

            core::App::setup();
        }

        void readSample()  // take 110 mcs
        {
            buffer.data[buffer.i++] = analogRead(Pin::adc);

            if (buffer.i == sizeof(buffer.data) / sizeof(int))
            {
                buffer.i = 0;

                setEvent(Event::BufferIsFull);
            }
        }

        void updateMetrics()
        {
            buffer.maxValue  = 0;
            buffer.minValue  = 1023;
            buffer.meanValue = 0;

            for (int i = 0; i < sizeof(buffer.data) / sizeof(int); ++i)
            {
                if (buffer.data[i] < buffer.minValue)
                {
                    buffer.minValue = buffer.data[i];
                }

                if (buffer.data[i] > buffer.maxValue)
                {
                    buffer.maxValue = buffer.data[i];
                }

                buffer.meanValue += buffer.data[i];
            }

            buffer.meanValue /= sizeof(buffer.data) / sizeof(int);
        }

        void paint() // take 31 ms
        {    
            // line 1: Max
            _display.setCursor(0, 0);
            _display.print("Max.  ");
            _display.print(buffer.maxValue);
            _display.print("    ");

            // line 2: Min
            _display.setCursor(0, 1); 
            _display.print("Min.  ");
            _display.print(buffer.minValue);
            _display.print("    ");

            // line 3: Mean
            _display.setCursor(0, 2);
            _display.print("Mean. ");
            _display.print(buffer.meanValue);
            _display.print("    ");
        }

    private:
        GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> _display;

        struct
        {
            int data[32];
            int i;
            int maxValue;
            int minValue;
            int meanValue;
        } buffer;
} app;

run(app);


