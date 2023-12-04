#include "Cuckoo.h"

Cuckoo::Cuckoo(){}

Cuckoo::~Cuckoo(){}

shared_ptr<filter> Cuckoo::initFilter(unsigned int capacity) {
    shared_ptr<filter> ret(new filter);
    ret->numBuckets = capacity;
    ret->table = shared_ptr<hash_table>(new hash_table(capacity));

    return ret;
}

shared_ptr<node> Cuckoo::initNode(unsigned int hash) {
    shared_ptr<node> ret(new node);

    return ret;
}

void insert(shared_ptr<filter> filter, std::string input, unsigned int capacity) {
    // C++'s built-in hash method is super useful for something like this; its output is size_t,
    // which is probably honestly too big for something as space-efficient as we're trying to be
    // with the Cuckoo Filter. Nevertheless, I will lazily incorporate the tools I have on hand
    // rather than trying to reinvent the wheel.
    size_t hash = std::hash<std::string>{}(input);
    
    // The concept of a "fingerprint" is basically some programmatically deterministic portion
    // of an input hash. In this case I want to use the last 16 bits of the size_t hash.
    uint16_t fingerprint = hash & 0xFFFF;
    int idx1 = fingerprint % capacity;
    int idx2 = fingerprint;
}

void remove(shared_ptr<filter> filter, std::string input) {}
bool contains(shared_ptr<filter> filter, std::string input) {}
int count(shared_ptr<filter> filter) {}
float capacityPct(shared_ptr<filter> filter) {}

uint16_t fingerprint(size_t hash) {}
