#ifndef SdHelper_h
#define SdHelper_h

class SDHelperClass {
  public:
    void begin(int pin);
    void dumpFile(char* path);
    bool deleteFile(char* path);
    void wipeCard();
  private:
    int _pin;
};

extern SDHelperClass SDHelper;

#endif
