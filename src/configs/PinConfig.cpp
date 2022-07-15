#define DEFAULT_LED 2
#define SS_PIN 5
#define RST_PIN 22

class PinConfigs {

    public:
        static int const defaultLed = DEFAULT_LED;
        static int const dataPin = SS_PIN;
        static int const resetPin = RST_PIN;
};