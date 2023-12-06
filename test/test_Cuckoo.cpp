// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/Cuckoo.h"

#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>

#define private public

using namespace std;

class test_Cuckoo : public ::testing::Test {
protected:
    // This function runs only once before any TEST_F function
	static void SetUpTestCase(){}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {}
};

// I have borrowed this random string generator from our Hashes homework (PA7)
std::string random_string(size_t length) {
  auto randchar = []() -> char {
    const char charset[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
  };
  std::string str(length,0);
  std::generate_n(str.begin(), length, randchar);
  return str;
}

// Unit Tests

TEST_F(test_Cuckoo, initFilter) {
    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);
    ASSERT_TRUE(f);
    ASSERT_EQ(f->capacity, 100);
    ASSERT_EQ(f->maxBucketKicks, 500);
    ASSERT_EQ(f->nodesPerBucket, 10);
    ASSERT_EQ(f->numBuckets, 10);
}

TEST_F(test_Cuckoo, initNode) {
    Cuckoo c;
    uint16_t fingerprint = (uint16_t) rand();
    shared_ptr<node> n = c.initNode(fingerprint);

    ASSERT_TRUE(n);
    ASSERT_EQ(n->fingerprint, fingerprint);
}

TEST_F(test_Cuckoo, insert) {
    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);
    c.insert(f, "https://www.google.com");

    ASSERT_TRUE(c.contains(f, "https://www.google.com"));
    ASSERT_EQ(f->size, 1);

    // If you try to insert a duplicate, expected behavior is to ignore it
    c.insert(f, "https://www.google.com");
    ASSERT_TRUE(c.contains(f, "https://www.google.com"));
    ASSERT_EQ(f->size, 1);
}

TEST_F(test_Cuckoo, insert_contains_remove) {
    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);

    c.insert(f, "https://www.google.com");
    ASSERT_TRUE(c.contains(f, "https://www.google.com"));
    ASSERT_FALSE(c.contains(f, "Fakey McFacke URL"));

    c.remove(f, "https://www.google.com");
    ASSERT_FALSE(c.contains(f, "https://www.google.com"));
    ASSERT_EQ(f->size, 0);

    // If the filter doesn't have the entry we tell it to remove, it should do nothing
    ASSERT_NO_FATAL_FAILURE(c.remove(f, "https://www.google.com"));
}

TEST_F(test_Cuckoo, fill_table) {
    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);
    
    // Insert the same string twice; this should result in us putting the value first
    // in a bucket with an index that's a hash of its fingerprint against the capacity,
    // and second (as a duplicate) in a bucket whose index is the first bucket's index
    // bitwise-XOR'd against the fingerprint itself hashed to the capacity size.
    for (int i = 0; i < 100; i++) {
        string randstr = "";
        randstr += (rand() % 500000);
        c.insert(f, randstr);
    }

    ASSERT_GE(f->size, 90);
}

TEST_F(test_Cuckoo, capacity_pct) {
    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);
    c.insert(f, "https://www.google.com");

    ASSERT_EQ(c.capacityPct(f), (float) 0.01);
}

// We need to test that we can deterministically remove things that have been kicked to some other bucket.
// This first adds enough items to the filter such that we should comfortably have done at least one kick,
// then shuffles the order of the items we added and removes them all. Should result in an empty filter.
TEST_F(test_Cuckoo, url_add_and_remove) {
    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);
    
    vector<string> urls;
    urls.push_back("https://google.com");
    urls.push_back("https://cisco.com");
    urls.push_back("https://purple.com");
    urls.push_back("https://theverge.com");
    urls.push_back("https://gizmodo.com");
    urls.push_back("https://meraki.com");
    urls.push_back("https://apple.com");
    urls.push_back("https://colorado.edu");
    urls.push_back("https://cam.ac.uk");
    urls.push_back("https://ox.ac.uk");
    urls.push_back("https://facebook.com");
    urls.push_back("https://instagram.com");
    urls.push_back("https://jalopnik.com");
    urls.push_back("https://caranddriver.com");
    urls.push_back("https://disney.com");
    urls.push_back("https://hulu.com");
    urls.push_back("https://max.com");
    urls.push_back("https://github.com");
    urls.push_back("https://gitlab.com");
    urls.push_back("https://digitalocean.com");
    urls.push_back("https://stackoverflow.com");
    urls.push_back("https://reddit.com");
    urls.push_back("https://hackernews.com");
    urls.push_back("https://nytimes.com");
    urls.push_back("https://zillow.com");
    urls.push_back("https://mergerecords.com");
    urls.push_back("https://youtube.com");
    urls.push_back("https://maps.google.com");
    urls.push_back("https://piazza.com");
    urls.push_back("https://cornell.edu");
    urls.push_back("https://colgate.edu");
    urls.push_back("https://harvard.edu");
    urls.push_back("https://dartmouth.edu");
    urls.push_back("https://yale.edu");
    urls.push_back("https://brown.edu");
    urls.push_back("https://berkeley.edu");
    urls.push_back("https://ucla.edu");
    urls.push_back("https://stanford.edu");
    urls.push_back("https://umichigan.edu");
    urls.push_back("https://overstock.com");
    urls.push_back("https://bsky.app");
    urls.push_back("https://chase.com");
    urls.push_back("https://wellsfargo.com");
    urls.push_back("https://amazon.com");
    urls.push_back("https://netflix.com");
    urls.push_back("https://schwab.com");
    urls.push_back("https://patreon.com");
    urls.push_back("https://mst3k.com");
    urls.push_back("https://sunblink.com");
    urls.push_back("https://ancestry.com");
    urls.push_back("https://duolingo.com");
    urls.push_back("https://adobe.com");
    urls.push_back("https://united.com");
    urls.push_back("https://battle.net");
    urls.push_back("https://steampowered.com");
    urls.push_back("https://nebula.tv");
    urls.push_back("https://mlb.com");
    urls.push_back("https://nfl.com");
    urls.push_back("https://espn.com");
    urls.push_back("https://motortrend.com");
    urls.push_back("https://googleplex.com");
    urls.push_back("https://substack.com");
    urls.push_back("https://mit.edu");
    urls.push_back("https://wikipedia.com");
    urls.push_back("https://xkcd.com");
    urls.push_back("https://npr.org");
    urls.push_back("https://salon.com");
    urls.push_back("https://rollingstone.com");
    urls.push_back("https://cppreference.com");
    urls.push_back("https://princeton.edu");
    urls.push_back("https://n26.com");
    urls.push_back("https://muenchen.de");
    urls.push_back("https://liquipedia.net");
    urls.push_back("https://nilenet.com");
    urls.push_back("https://sueddeutsche.de");
    urls.push_back("https://nottingham.ac.uk");
    urls.push_back("https://durham.ac.uk");
    urls.push_back("https://disneyplus.com");
    urls.push_back("https://tum.de");
    
    for (int i = 0; i < urls.size(); i++) {
        c.insert(f, urls[i]);
    }
    
    ASSERT_EQ(f->size, urls.size());
    
    // Shuffle input list
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(urls.begin(), urls.end(), g);

    int size = urls.size();
    for (int i = 0; i < size; i++) {
        string to_remove = urls.at(i);
        c.remove(f, to_remove);
    }

    ASSERT_EQ(f->size, 0);
}