# Cuckoo Filter
#### CS2270 F23 Final Project
#### Mallory Beru | Identikey: mabe9211

## Data Structure Overview
A Cuckoo Filter is a Filter with some unique properties.

The purpose of a Filter is essentially to store hashed input, in the same way that a Hash Table does. However, unlike a Hash Table, a Filter is not designed for us to retrieve data after insertion; rather, a filter gives us the ability to be able to easily discern whether a given input was stored in the Filter at some point. It saves the values as hashes (or, in our case, uniformly-sized truncated versions of hashes, which we'll call fingerprints) in order to provide this funcationality for a very large number of inputs in a relatively small space.

What makes a Cuckoo Filter a Cuckoo Filter is the peculiar process why which insertion and removal are accomplished. I assume (though I haven't seen this in the literature I've read) that it's called a "Cuckoo Filter" because collisions are resolved by evicting the current resident of a bucket and replacing its contents with our own. (The cuckoo bird kicks other birds's eggs out of their nests and replaces them with its own; this is also the etymological basis of the term "cuckold", FYI! The More You Know.)

### The Cuckoo Insert/Remove Algorithm (Partial-key Cuckoo Hashing)
#### Insert
Insert and Remove are accomplished in a really cool way. Each value that's to be inserted has two candidate buckets:
1. First, we hash our input string with std::hash<string> (after we take this hash, we proceed to take a "fingerprint" of it; the fingerprint is a truncated version of the hash--in our case, just the last 16 bits of it, which we store as a uint16_t ("unsigned short")). We then modulo this hash by the number of buckets to get the first index candidate (i.e., one of two possible indices representing a bucket our value could be inserted into in an array of buckets).
2. Next, we take a second hash, and it is the result of doing a bitwise XOR with that first candidate index on the left and a std::hash<unsigned short> of the **fingerprint** on the right. We modulo that by the number of buckets to get a second candidate index (which is not necessarily different from the first one, especially with smaller numbers of buckets).

Doing this with the XOR and the fingerprint as we do is crucial, as you will see in a moment. Note also that the fingerprint generated in step 1 is the thing we will actually insert into the Filter.

So: every filter we want to insert into our Cuckoo Filter has two possible buckets it could be inserted into, each represented by one of those indices. If either of those buckets has room for a new entry, we insert the fingerprint into that bucket. But if nether does, then we start the Cuckoo eviction process.

We randomly select one of the two indices we calculated in the first part of the insertion, and then we randomly pick one of the slots in the bucket at that index. We then swap the fingerprint we want to store with the fingerprint already stored in that random slot. Finally, we calculate the _other eligible bucket_ for the random fingerprint we picked (this is why the cuckoo algorithm is so cool--we just do another bitwise XOR on a std::hash of the fingerprint itself and modulo it by the length of the bucket; this gives us the other valid index for that fingerprint's insertion). If that other bucket option has a free slot, great! We store our value there. Else, we start again (picking a random value in our chosen bucket, swapping its fingerprint with ours, and finding its alternative bucket).

We try this process a total of `maxBucketKicks` (a parameter of the Filter) times; if it doesn't work after that many attempts, we should really probably throw an error, but in the case of this project, we fail silently.

#### Remove
This all makes Remove really simple by comparison; for any given value, we know that its fingerprint will reside in one of its two eligible buckets, so we look in both of them to see if the fingerprint is there; if so, we mark that slot in the bucket `removed`.

## Use Cases
I have used the app/main.cpp file for a small walkthrough of how a Cuckoo Filter might be used for URL filtering (in e.g. a browser or a network appliance that does L7 flow inspection). You can run this for yourself by cloning this repo and following the standard workflow we used throughout the course:
1. `cd ./build && cmake ..`
2. `make`
3. `./run_app`

**...OR...** if you don't want to clone this and build/run all that yourself, you can go to [GitHub Actions](https://github.com/scooberu/cs2270-finalproject/actions) for this project and click on the latest (top) Workflow; it will contain a stage for `Run` that shows the full output of running `./run_app` on GitHub's architecture.
