#ifndef SdHelper_h
#define SdHelper_h

class SDHelperClass {
  public:
    void begin(int pin);
    void dumpFile(char* path);
    void wipeCard();
};

extern SDHelperClass SDHelper;

#endif
