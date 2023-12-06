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
    bool removed;
};

typedef vector<shared_ptr<node>> bucket;
typedef vector<shared_ptr<bucket>> hash_table;

struct filter {
    shared_ptr<hash_table> table;
    unsigned int numBuckets;
    unsigned int nodesPerBucket;
    unsigned int capacity;
    int size;
    unsigned int maxBucketKicks;
};

class Cuckoo {
public:
    Cuckoo();
    ~Cuckoo();
    shared_ptr<filter> initFilter(unsigned int maxBucketKicks, unsigned int numBuckets, unsigned int nodesPerBucket);
    shared_ptr<node> initNode(uint16_t fingerprint);
    void insert(shared_ptr<filter> filter, string input);
    void remove(shared_ptr<filter> filter, string input);
    bool contains(shared_ptr<filter> filter, string input);
    float capacityPct(shared_ptr<filter> filter);
    void printFilter(shared_ptr<filter> filter);
private:
    uint16_t fingerprint(size_t hash);
    size_t str_hash(string input);
    size_t short_hash(uint16_t input);
    shared_ptr<node> find(shared_ptr<filter> filter, string input);
    bool nodeIsEmpty(shared_ptr<node> n);
    bool bucketHasRoom(shared_ptr<filter> filter, int bucket_idx);
    bool bucketContains(shared_ptr<filter> filter, int bucket_idx, uint16_t f);
    shared_ptr<node> bucketFind(shared_ptr<filter> filter, int bucket_idx, uint16_t f);
    void addToBucket(shared_ptr<filter> filter, int bucket_idx, uint16_t f);
    void printBucket(shared_ptr<bucket> bucket);
};

#endif  // CUCKOO_H__