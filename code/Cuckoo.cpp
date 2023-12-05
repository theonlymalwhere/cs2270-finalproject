#include "Cuckoo.h"

Cuckoo::Cuckoo(){}
Cuckoo::~Cuckoo(){}

shared_ptr<filter> Cuckoo::initFilter(unsigned int capacity, unsigned int maxBucketKicks) {
    shared_ptr<filter> ret(new filter);
    ret->capacity = capacity;
    ret->fullBuckets = 0;
    ret->table = shared_ptr<hash_table>(new hash_table(capacity));
    ret->maxBucketKicks = maxBucketKicks;

    return ret;
}

shared_ptr<node> Cuckoo::initNode(uint16_t fingerprint) {
    shared_ptr<node> ret(new node);
    ret->fingerprint = fingerprint;
    ret->removed = false;

    return ret;
}

void Cuckoo::insert(shared_ptr<filter> filter, string input) {
    if (filter->fullBuckets >= filter->capacity) { return; }

    auto h = str_hash(input);
    auto f = fingerprint(h);

    auto idx1 = h % filter->capacity;
    auto idx2 = idx1 ^ (short_hash(f) % filter->capacity);
    int free_idx; // in case we need to kick entries around

    shared_ptr<node> newnode = initNode(f);

    if (nodeIsEmpty(filter->table->at(idx1))) {
        filter->table->at(idx1) = newnode;
        filter->fullBuckets++;
        return;
    } else if (nodeIsEmpty(filter->table->at(idx2))) {
        filter->table->at(idx2) = newnode;
        filter->fullBuckets++;
        return;
    } else { // Have to kick an existing item to another bucket
        // Randomly select a 0 or a 1. We need this to be able to pick which
        // of the two index types we'll be evicting.
        int kick_idx = (rand() % 2 == 0) ? idx1 : idx2;
        free_idx = kickToOtherBucket(filter, kick_idx);
        if (free_idx >= 0) {
            filter->table->at(free_idx) = newnode;
            filter->fullBuckets++;
        }
    }
}

void Cuckoo::remove(shared_ptr<filter> filter, string input) {
    auto existing_node = find(filter, input);
    if (existing_node != nullptr) {
        existing_node->removed = true;
        filter->fullBuckets--;
    }
}

bool Cuckoo::contains(shared_ptr<filter> filter, string input) {
    auto h = str_hash(input);
    auto f = fingerprint(h);

    auto idx1 = h % filter->capacity;
    auto idx2 = idx1 ^ (short_hash(f) % filter->capacity);

    if (!nodeIsEmpty(filter->table->at(idx1)) && filter->table->at(idx1)->fingerprint == f) {
        return true;
    } else if (!nodeIsEmpty(filter->table->at(idx2)) && filter->table->at(idx2)->fingerprint == f) {
        return true;
    } else {
        return false;
    }
}

float Cuckoo::capacityPct(shared_ptr<filter> filter) {
    return (filter->fullBuckets / filter->capacity);
}

uint16_t Cuckoo::fingerprint(size_t hash) {
    // The concept of a "fingerprint" is basically some set-sized sub-portion of an
    // input hash. In this case I want to use the last 16 bits of the size_t hash.
    return hash & 0xFFFF;
}

size_t Cuckoo::str_hash(string input) {
    // C++'s built-in hash method is super useful for something like this; its output is size_t,
    // which is probably honestly too big for something as space-efficient as we're trying to be
    // with the Cuckoo Filter. Nevertheless, I will lazily incorporate the tools I have on hand
    // rather than trying to reinvent the wheel.
    return std::hash<string>{}(input);
}

size_t Cuckoo::short_hash(uint16_t input) {
    // Ditto but we need a different mechanism to re-hash an existing fingerprint
    return std::hash<uint16_t>{}(input);
}

bool Cuckoo::nodeIsEmpty(shared_ptr<node> n) {
    return n == nullptr || n->removed;
}

int Cuckoo::kickToOtherBucket(shared_ptr<filter> filter, int srcIndex) {
    int kick = 0;
    int i = srcIndex;
    auto fingerprint_to_insert = filter->table->at(srcIndex)->fingerprint;

    while (kick < filter->maxBucketKicks) {
        int rand_idx = rand() % filter->capacity;
        std::swap(filter->table->at(rand_idx), filter->table->at(i));

        i = i ^ (short_hash(fingerprint_to_insert) % filter->capacity);
        if (nodeIsEmpty(filter->table->at(i))) { return i; }
        kick++;
    }
    return -1; // No other bucket found
}

shared_ptr<node> Cuckoo::find(shared_ptr<filter> filter, std::string input) {
    auto h = str_hash(input);
    auto f = fingerprint(h);

    auto idx1 = h % filter->capacity;
    auto idx2 = idx1 ^ (short_hash(f) % filter->capacity);

    if (filter->table->at(idx1)->fingerprint == f && !filter->table->at(idx1)->removed) {
        return filter->table->at(idx1);
    } else if (filter->table->at(idx2)->fingerprint == f && !filter->table->at(idx2)->removed) {
        return filter->table->at(idx2);
    } else {
        return nullptr;
    }
}
