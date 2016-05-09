#ifndef ORACLE_H
#define ORACLE_H

class OracleHeavyVids
{
    public:
        OracleHeavyVids();
        OracleHeavyVids(const char* filename, int threshold);

        // decides whether a video is computable or not
        bool decide();

    private:
        bool heavy;
};

#endif // ORACLE_H
