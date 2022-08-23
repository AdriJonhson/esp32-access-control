#ifndef USER_SERVICE
#define USER_SERVICE

#include <Arduino.h>

class UserService {
    public:
        void verifyUserAccess(char *rfidCode);
        void sendRfidCode(char *registerIdentify, char *rfidCode);

};

#endif
