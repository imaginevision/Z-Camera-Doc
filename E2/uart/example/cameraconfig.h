#ifndef _CAMERA_CONFIG_H_
#define _CAMERA_CONFIG_H_

#include <map>

typedef std::map<int, const char *> Map;
typedef std::pair<int, const char *> Entry;
typedef std::map<int, int> IntMap;
typedef std::pair<int, int> IntEntry;

class CameraConfig {
public:
    static void init();
    static const char* trKeyFromInt(int key);
    static const char* trValueFromInt(int key, int value);
    static unsigned int trKeyFromString(const char *key);
    static int trValueFromString(unsigned int key, const char *value);
    static const char* trReplace(const char * str, const char * pattern,  const char *target, char* buf,unsigned int len);

    static int trValueFromIndex(int key, int index);
private:
    static Map* getMapBykey(int key);
    static IntMap *getIntMapByKey(int key);
};

#endif
