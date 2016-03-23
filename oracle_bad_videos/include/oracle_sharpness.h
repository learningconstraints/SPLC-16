#ifndef ORACLE_SHARPNESS_H
#define ORACLE_SHARPNESS_H

#include <oracle.h>

class oracle_sharpness : public oracle
{
    public:
        oracle_sharpness();
        oracle_sharpness(string fname, int f=10);
        virtual ~oracle_sharpness();
        bool decide();
    protected:
    private:
};

#endif // ORACLE_SHARPNESS_H
