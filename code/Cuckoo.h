#ifndef CUCKOO_H__
#define CUCKOO_H__

#include <string>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// This struct defines a container for the filter; each bucker will have nodesPerBucket of these
struct node {
    uint16_t fingerprint;
    bool removed;
};

// A bucket is a vector of pointers to nodes
typedef vector<shared_ptr<node>> bucket;
// A hash_table (ugh) is a vector of pointers to buckets
typedef vector<shared_ptr<bucket>> hash_table;

// This struct is the functional unit of the Cuckoo class, really
struct filter {
    // The filter's table has all of its buckets, each of which is a vector of nodes
    shared_ptr<hash_table> table;
    // Number of buckets the table should have
    unsigned int numBuckets;
    // Number of nodes each bucket should have
    unsigned int nodesPerBucket;
    // Total capcity of the filter
    unsigned int capacity;
    // Total occupancy of the filter
    int size;
    // Max number of tries for us to emplace a new item in the filter via the cuckoo kick mechanism
    unsigned int maxBucketKicks;
};

class Cuckoo {
public:
    // Default empty constructor and destructor are fine for us because the filter does most of the work
    Cuckoo();
    ~Cuckoo();
    // Creates a filter struct according to given values
    shared_ptr<filter> initFilter(unsigned int maxBucketKicks, unsigned int numBuckets, unsigned int nodesPerBucket);
    // Creates a node to fill one vacant slot in a bucket
    shared_ptr<node> initNode(uint16_t fingerprint);
    // Insert is the most complicated algorithm in this data structure. The basic structure of it is adapted
    // from descriptions and pseudocode found in
    // https://www.eecs.harvard.edu/~michaelm/postscripts/cuckoo-conext2014.pdf. But essentially, given an input
    // string (e.g., a URL to be inserted into the filter), we check whether the filter is full or has the input
    // in an anticipated place already; if not, we proceed to insert.
    //
    // Insert itself initially makes two different hashes; the first is std::hash<string> (After we take this hash,
    // we proceed to take a "fingerprint" of it; the fingerprint is a truncated version of the hash--in our case,
    // just the last 16 bits of it). We then modulo this hash by the number of buckets to get the first index
    // candidate (i.e., one of two possible indices representing a bucket our value could be inserted into).
    //
    // The second hash is the result of doing a bitwise XOR with that first candidate index on the left and a
    // std::hash<unsigned short> of the **fingerprint** on the right. We modulo that by the number of buckets to
    // get a second candidate index (not necessarily different from the first one, especially with smaller numbers
    // of buckets).
    //
    // Initially we try to insert our value in the bucket indicated by each of these indices (in order). If
    // neither has a slot open for our value, we must do the "cuckoo" part of the filter. (I assume this is
    // named for the bird because they lay their eggs in other birds' nests, kicking out the eggs of the
    // birds who made those nests). This process works in the following way: we randomly select one of the two
    // indices we figured out in the first part of the insertion, and then we randomly pick one of the slots
    // in the bucket at that index. We then swap the fingerprint for the value we want to store with the
    // fingerprint in that random slot. Finally, we calculate the other eligible bucket for that fingerprint
    // (this is why the cuckoo algorithm is so cool--we just do another bitwise XOR on a std::hash of the
    // fingerprint itself and modulo it by the length of the bucket; this gives us the other valid index for
    // that fingerprint's insertion). If that other bucket option has a free slot, great! We store our value there.
    // Else, we start again (picking a random value in our chosen bucket, swapping its fingerprint with ours,
    // and finding its alternative bucket).
    //
    // We try this process a total of maxBucketKicks times; if it doesn't work, we should really probably throw
    // an error, but in the case of this final project, we fail silently.
    void insert(shared_ptr<filter> filter, string input);
    // remove() is really easy by comparison; we know that a given fingerprint will be in one of its two valid
    // buckets. So we check if the value is in either bucket; if it is, we mark that slot removed. Else, we
    // fail silently.
    void remove(shared_ptr<filter> filter, string input);
    // For contains(), we again do our two hashes on the input string and then check all the slots in both
    // buckets to determine whether any non-removed bucket contains the fingerprint of the input string.
    bool contains(shared_ptr<filter> filter, string input);
    // This is mostly a utility for my benefit; theoretically it should be possible for a cuckoo filter to get
    // to about 95% full (see the paper linked above). I wanted to check this, so I exposed a public method for
    // users to be able to check occupancy as well.
    float capacityPct(shared_ptr<filter> filter);
    // This is also a utility I added for my own benefit. It prints all the contents of a cuckoo filter in
    // the following format:
    /*
    FILTER CONTENTS
    ===============
        BUCKET 0 CONTENTS:
        [0]: 30710
        [1]: 51808
        [2]: 31868
        [3]: 17624
        [4]: 14778
        [5]: 20408
        [6]: 54740
        [7]: 17624
        [8]: [REMOVED]
        [9]: [EMPTY SINCE INIT]
    */
    void printFilter(shared_ptr<filter> filter);
private:
    // Given an input hash (these are size_t), truncate it to provide the last 16 bits.
    uint16_t fingerprint(size_t hash);
    // Run std::hash<string> on a given input string.
    size_t str_hash(string input);
    // Run std::hash<uint16_t> on a given input unsighned short.
    size_t short_hash(uint16_t input);
    // remove() needs this utility to find the actual node that has a given input string.
    // Returns nullptr if not found.
    shared_ptr<node> find(shared_ptr<filter> filter, string input);
    // Utility; lots of thinds in this code need to know if a given node is either empty since init or removed.
    // This returns true if either is the case.
    bool nodeIsEmpty(shared_ptr<node> n);
    // Primarily used for insert(), we need bucketHasRoom() to be able to know if a a bucket has free space,
    // which means essentially "nodeIsEmpty() is true for any node in a given bucket."
    bool bucketHasRoom(shared_ptr<filter> filter, int bucket_idx);
    // This is a helper method for contains() just iterates through a given bucket and returns true if any
    // non-removed node has the given fingerprint f.
    bool bucketContains(shared_ptr<filter> filter, int bucket_idx, uint16_t f);
    // This is a helper methid for find(); basically the same thing as bucketContains but returns the node
    // itself if it's there instead of a boolean. As bucketContains() is to contains(), this is to find().
    shared_ptr<node> bucketFind(shared_ptr<filter> filter, int bucket_idx, uint16_t f);
    // This is a helper method for insert(); I noticed this action was bulky so I packaged it into a separate
    // function. Insert looks different depending on whether the target bucket is nullptr or an existing node
    // that has removed == true; this handles both conditions seamlessly to callers.
    void addToBucket(shared_ptr<filter> filter, int bucket_idx, uint16_t f);
    // This is a helper method for printFilter(); printFilter() prints every bucket's contents, so I split
    // the bucket printing facillity into a method that can be looped easiy.
    void printBucket(shared_ptr<bucket> bucket);
};

#endif  // CUCKOO_H__