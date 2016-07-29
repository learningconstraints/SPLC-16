#ifndef ORACLE_H
#define ORACLE_H

#define THRESHOLD 17000000

// This oracle declares videos faulty if their size (in bytes) exceed the threshold
class Oracle {
    public:
        Oracle();
        Oracle(const char* video_path);
        bool decide();
    private:
        bool heavy;
};

#endif // ORACLE_H
