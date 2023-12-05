#ifndef CUCKOO_H__
#define CUCKOO_H__

#include <string>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct node {
    uint16_t fingerprint;
};

typedef vector<shared_ptr<node>> hash_table;

struct filter {
    shared_ptr<hash_table> table;
    unsigned int capacity;
    int fullBuckets;
    int fingerprintLength;
    unsigned int maxBucketKicks;
};

class Cuckoo {
public:
    Cuckoo();
    ~Cuckoo();
    shared_ptr<filter> initFilter(unsigned int capacity, unsigned int maxBucketKicks);
    shared_ptr<node> initNode(uint16_t fingerprint);
    void insert(shared_ptr<filter> filter, std::string input);
    void remove(shared_ptr<filter> filter, std::string input);
    bool contains(shared_ptr<filter> filter, std::string input);
    float capacityPct(shared_ptr<filter> filter);
private:
    uint16_t fingerprint(size_t hash);
    size_t str_hash(std::string input);
    size_t short_hash(uint16_t input);
    int kickToOtherBucket(shared_ptr<filter> filter, int srcIndex);
    shared_ptr<node> find(shared_ptr<filter> filter, std::string input);
};

#endif  // CUCKOO_H__