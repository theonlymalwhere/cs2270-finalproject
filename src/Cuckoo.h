#ifndef CUCKOO_H__
#define CUCKOO_H__

#include <string>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct node {
    unsigned int fingerprint;
    std::string value;
};

typedef vector<shared_ptr<node>> hash_table;

struct filter {
    shared_ptr<hash_table> table;
    int numBuckets;
    int fingerprintLength;
    int maxBucketKicks;
};

class Cuckoo {
public:
    Cuckoo();
    ~Cuckoo();
    shared_ptr<filter> initFilter(unsigned int capacity);
    shared_ptr<node> initNode(unsigned int hash);
    void insert(shared_ptr<filter> filter, std::string input, unsigned int capacity);
    void remove(shared_ptr<filter> filter, std::string input);
    bool contains(shared_ptr<filter> filter, std::string input);
    int count(shared_ptr<filter> filter);
    float capacityPct(shared_ptr<filter> filter);
private:
    uint16_t fingerprint(size_t hash);

};

#endif  // CUCKOO_H__