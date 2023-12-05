#include "Cuckoo.h"

Cuckoo::Cuckoo(){}
Cuckoo::~Cuckoo(){}

shared_ptr<filter> Cuckoo::initFilter(unsigned int maxBucketKicks, unsigned int numBuckets, unsigned int nodesPerBucket) {
    shared_ptr<filter> ret(new filter);
    ret->capacity = (numBuckets * nodesPerBucket);
    ret->size = 0;
    ret->nodesPerBucket = nodesPerBucket;
    ret->numBuckets = numBuckets;
    ret->maxBucketKicks = maxBucketKicks;

    ret->table = shared_ptr<hash_table>(new hash_table(numBuckets));
    int i = 0;
    while (i < numBuckets) {
        ret->table->at(i) = shared_ptr<bucket>(new bucket(nodesPerBucket));
        i++;
    }
    
    return ret;
}

shared_ptr<node> Cuckoo::initNode(uint16_t fingerprint) {
    shared_ptr<node> ret(new node);
    ret->fingerprint = fingerprint;
    ret->removed = false;

    return ret;
}

void Cuckoo::insert(shared_ptr<filter> filter, string input) {
    if (filter->size >= filter->capacity) { return; }

    auto h = str_hash(input);
    auto f = fingerprint(h);

    auto idx1 = h % filter->numBuckets;
    auto idx2 = idx1 ^ (short_hash(f) % filter->numBuckets);

    if (bucketHasRoom(filter, idx1)) {
        addToBucket(filter, idx1, f);
        return;
    } else if (bucketHasRoom(filter, idx2)) {
        addToBucket(filter, idx2, f);
        return;
    } else { // Have to kick an existing item to another bucket or slot in this bucket
        // Randomly select a 0 or a 1. We need this to be able to pick which
        // of the two index types we'll be evicting.
        int kick_idx = (rand() % 2 == 0) ? idx1 : idx2;
        //free_idx = kickBucket(filter, kick_idx);

        int kick = 0;
        while (kick < filter->maxBucketKicks) {
            int rand_idx = rand() % filter->nodesPerBucket;
            uint16_t tmp = filter->table->at(kick_idx)->at(rand_idx)->fingerprint;
            filter->table->at(kick_idx)->at(rand_idx)->fingerprint = f;
            f = tmp;

            kick_idx = kick_idx ^ (short_hash(f) % filter->numBuckets);
            if (bucketHasRoom(filter, kick_idx)) {
                addToBucket(filter, kick_idx, f);
            }
            kick++;
        }
    }
}

void Cuckoo::remove(shared_ptr<filter> filter, string input) {
    auto existing_node = find(filter, input);
    if (existing_node != nullptr) {
        existing_node->removed = true;
        filter->size--;
    }
}

bool Cuckoo::contains(shared_ptr<filter> filter, string input) {
    auto h = str_hash(input);
    auto f = fingerprint(h);

    auto idx1 = h % filter->numBuckets;
    auto idx2 = idx1 ^ (short_hash(f) % filter->numBuckets);

    return (bucketContains(filter, idx1, f) || bucketContains(filter, idx2, f) );
}

float Cuckoo::capacityPct(shared_ptr<filter> filter) {
    return (filter->size / filter->capacity);
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

bool Cuckoo::bucketHasRoom(shared_ptr<filter> filter, int bucket_idx) {
    auto b = filter->table->at(bucket_idx);
    for (int i = 0; i < b->size(); i++) {
        if (nodeIsEmpty(b->at(i))) {
            return true;
        }
    }
    return false;
}

bool Cuckoo::bucketContains(shared_ptr<filter> filter, int bucket_idx, uint16_t f) {
    auto b = filter->table->at(bucket_idx);
    for (int i = 0; i < b->size(); i++) {
        if (!nodeIsEmpty(b->at(i))) {
            if (b->at(i)->fingerprint == f) {
                return true;
            }
        }
    }
    return false;
}

shared_ptr<node> Cuckoo::bucketFind(shared_ptr<filter> filter, int bucket_idx, uint16_t f) {
    auto b = filter->table->at(bucket_idx);
    for (int i = 0; i < b->size(); i++) {
        if (!nodeIsEmpty(b->at(i))) {
            if (b->at(i)->fingerprint == f) {
                return b->at(i);
            }
        }
    }
    return nullptr;
}

void Cuckoo::addToBucket(shared_ptr<filter> filter, int bucket_idx, uint16_t f) {
    auto b = filter->table->at(bucket_idx);
    for (int i = 0; i < b->size(); i++) {
        if (b->at(i) == nullptr) {
            b->at(i) = initNode(f);
            filter->size++;
            return;
        } else if (b->at(i)->removed) {
            b->at(i)->fingerprint = f;
            b->at(i)->removed = false;
            filter->size++;
            return;
        }
    }
}

shared_ptr<node> Cuckoo::find(shared_ptr<filter> filter, std::string input) {
    auto h = str_hash(input);
    auto f = fingerprint(h);

    auto idx1 = h % filter->numBuckets;
    auto idx2 = idx1 ^ (short_hash(f) % filter->numBuckets);

    auto ret = bucketFind(filter, idx1, f); // will be nullptr if not found

    if (ret != nullptr) {
        return ret;
    } else {
        return bucketFind(filter, idx2, f); // will be nullptr if not found
    }
}
