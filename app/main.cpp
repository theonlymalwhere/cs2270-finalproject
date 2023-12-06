#include <iostream>
#include <vector>
#include <chrono>
#include "../code/Cuckoo.h"

using namespace std;

int main(){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    Cuckoo c;
    shared_ptr<filter> f = c.initFilter(500, 10, 10);
    
    cout << "I want to demonstrate the utility of the Cuckoo Filter as the backend for URL filtering (deny/allowlisting)." << endl;
    cout << "First, I will add about 80 URLs to our Cuckoo Filter." << endl;
    cout << "To do this, I'm first going to make a vector and then iteratively grab each URL from it and add it to the Filter." << endl;
    vector<string> url_blocklist;
    url_blocklist.push_back("https://google.com");
    url_blocklist.push_back("https://cisco.com");
    url_blocklist.push_back("https://purple.com");
    url_blocklist.push_back("https://theverge.com");
    url_blocklist.push_back("https://gizmodo.com");
    url_blocklist.push_back("https://meraki.com");
    url_blocklist.push_back("https://apple.com");
    url_blocklist.push_back("https://colorado.edu");
    url_blocklist.push_back("https://cam.ac.uk");
    url_blocklist.push_back("https://ucl.ac.uk");
    url_blocklist.push_back("https://facebook.com");
    url_blocklist.push_back("https://instagram.com");
    url_blocklist.push_back("https://jalopnik.com");
    url_blocklist.push_back("https://caranddriver.com");
    url_blocklist.push_back("https://disney.com");
    url_blocklist.push_back("https://hulu.com");
    url_blocklist.push_back("https://max.com");
    url_blocklist.push_back("https://github.com");
    url_blocklist.push_back("https://gitlab.com");
    url_blocklist.push_back("https://digitalocean.com");
    url_blocklist.push_back("https://stackoverflow.com");
    url_blocklist.push_back("https://reddit.com");
    url_blocklist.push_back("https://hackernews.com");
    url_blocklist.push_back("https://nytimes.com");
    url_blocklist.push_back("https://zillow.com");
    url_blocklist.push_back("https://mergerecords.com");
    url_blocklist.push_back("https://youtube.com");
    url_blocklist.push_back("https://maps.google.com");
    url_blocklist.push_back("https://piazza.com");
    url_blocklist.push_back("https://cornell.edu");
    url_blocklist.push_back("https://colgate.edu");
    url_blocklist.push_back("https://harvard.edu");
    url_blocklist.push_back("https://dartmouth.edu");
    url_blocklist.push_back("https://yale.edu");
    url_blocklist.push_back("https://brown.edu");
    url_blocklist.push_back("https://berkeley.edu");
    url_blocklist.push_back("https://ucla.edu");
    url_blocklist.push_back("https://stanford.edu");
    url_blocklist.push_back("https://umichigan.edu");
    url_blocklist.push_back("https://overstock.com");
    url_blocklist.push_back("https://bsky.app");
    url_blocklist.push_back("https://chase.com");
    url_blocklist.push_back("https://wellsfargo.com");
    url_blocklist.push_back("https://amazon.com");
    url_blocklist.push_back("https://netflix.com");
    url_blocklist.push_back("https://schwab.com");
    url_blocklist.push_back("https://patreon.com");
    url_blocklist.push_back("https://mst3k.com");
    url_blocklist.push_back("https://sunblink.com");
    url_blocklist.push_back("https://ancestry.com");
    url_blocklist.push_back("https://duolingo.com");
    url_blocklist.push_back("https://adobe.com");
    url_blocklist.push_back("https://united.com");
    url_blocklist.push_back("https://battle.net");
    url_blocklist.push_back("https://steampowered.com");
    url_blocklist.push_back("https://nebula.tv");
    url_blocklist.push_back("https://mlb.com");
    url_blocklist.push_back("https://nfl.com");
    url_blocklist.push_back("https://espn.com");
    url_blocklist.push_back("https://motortrend.com");
    url_blocklist.push_back("https://googleplex.com");
    url_blocklist.push_back("https://substack.com");
    url_blocklist.push_back("https://mit.edu");
    url_blocklist.push_back("https://wikipedia.com");
    url_blocklist.push_back("https://xkcd.com");
    url_blocklist.push_back("https://npr.org");
    url_blocklist.push_back("https://salon.com");
    url_blocklist.push_back("https://rollingstone.com");
    url_blocklist.push_back("https://cppreference.com");
    url_blocklist.push_back("https://princeton.edu");
    url_blocklist.push_back("https://n26.com");
    url_blocklist.push_back("https://muenchen.de");
    url_blocklist.push_back("https://liquipedia.net");
    url_blocklist.push_back("https://nilenet.com");
    url_blocklist.push_back("https://sueddeutsche.de");
    url_blocklist.push_back("https://nottingham.ac.uk");
    url_blocklist.push_back("https://durham.ac.uk");
    url_blocklist.push_back("https://disneyplus.com");
    url_blocklist.push_back("https://tum.de");
    
    cout << "The following URLs are now in this filter:" << endl;
    for (int i = 0; i < url_blocklist.size(); i++) {
        cout << url_blocklist[i] << endl;
        c.insert(f, url_blocklist[i]);
    }
    
    cout << "\n\n\n...and the filter itself has the following contents:\n" << endl;
    c.printFilter(f);
    cout << "\n\n\n";

    int url_blocklist_vector_size = 0;
    for (const string& s: url_blocklist) {
        url_blocklist_vector_size += s.length();
    }

    int filter_size = (f->size * 2); // each entry in the filter is a 16-bit fingerprint, 16 bits is 2 bytes

    cout << "Already there is a storage advantage to the filter." <<endl;
    cout << "The vector of addresses uses " << url_blocklist_vector_size << " bytes of memory to store URLs themselves." << endl;
    cout << "The filter uses " << filter_size << " bytes of memory to store the same number of URLs (when I ran this on my laptop, there was 1 order of magnitude difference)." << endl;
    cout << "(This is irrespective of metadata in both data structures, and of course the reason we can save so much space is that in the filter, we can't get the raw URLs back out)\n" << endl;

    cout << "Let's imagine we want to use this filter as a URL blocklist." << endl;
    cout << "Let's assume that this filter is running on a network device somewhere that does L7 inspection of network traffic." << endl;
    cout << "I'll assume I got one GET request from a user for a blocked website (https://cam.ac.uk -- shoo the tabs!) and one for an allowed website (https://vimeo.com)" << endl;
    cout << "Ok, so the first thing I want to demonstrate is how much faster it is to find a URL in the filter vs by checking the vector." << endl;

    auto t1 = high_resolution_clock::now();
    auto cambridge_filter_blocked = c.contains(f, "https://cam.ac.uk");
    auto t2 = high_resolution_clock::now();

    auto t3 = high_resolution_clock::now();
    auto vimeo_filter_blocked = c.contains(f, "https://vimeo.com");
    auto t4 = high_resolution_clock::now();

    auto t5 = high_resolution_clock::now();
    auto cambridge_vector_blocked = false;
    for (int i = 0; i < url_blocklist.size(); i++) {
        if (url_blocklist[i] == "https://cam.ac.uk") {
            cambridge_vector_blocked = true;
        }
    }
    auto t6 = high_resolution_clock::now();

    auto t7 = high_resolution_clock::now();
    auto vimeo_vector_blocked = false;
    for (int i = 0; i < url_blocklist.size(); i++) {
        if (url_blocklist[i] == "https://vimeo.com") {
            vimeo_vector_blocked = true;
        }
    }
    auto t8 = high_resolution_clock::now();

    duration<double, std::milli> cambridge_filter_dur = t2 - t1;
    duration<double, std::milli> vimeo_filter_dur = t4 - t3;
    duration<double, std::milli> cambridge_vector_dur = t6 - t5;
    duration<double, std::milli> vimeo_vector_dur = t8 - t7;

    cout << "\nTime to check the vector for cam.ac.uk: " << cambridge_vector_dur.count() << " milliseconds" << endl;
    cout << "Time to check the vector for vimeo: " << vimeo_vector_dur.count() << " milliseconds" << endl;
    cout << "\nTime to check the filter for cam.ac.uk: " << cambridge_filter_dur.count() << " milliseconds" << endl;
    cout << "Time to check the filter for vimeo: " << vimeo_filter_dur.count() << " milliseconds" << endl;

    cout << "\n\nYMMV depending on platform etc., but in my experience, checking the filter's contents is 1 order of magnitude faster (0.00n ms vs. 0.000n ms)." << endl;

    cout << "Now, the fun thing about Cuckoo Filters specifically is that the hash method (unlike a Bloom Filter) also lets us remove entries." << endl;
    cout << "So let's assume that we find it in our hearts to allow users to access the Univ. of Cambridge website." << endl;
    cout << "We can remove it from the filter **without recompiling the whole thing** like a Bloom Filter (woohoo!)." << endl;
    cout << "And, because the location of cam.ac.uk is deterministic (O(# elements per bucket) access time), removal is FAST!\n" << endl;

    auto t9 = high_resolution_clock::now();
    c.remove(f, "https://cam.ac.uk");
    auto t10 = high_resolution_clock::now();

    duration<double, std::milli> cambridge_remove_dur = t10 - t9;
    cout << "Time to remove cam.ac.uk from the blocklist: " << cambridge_remove_dur.count() << " milliseconds" << endl;
    cout << "(^^^ should be about the same amount of time as it took us to check whether present/absent items were in the filter)" << endl;

    c.remove(f, "https://sueddeutsche.de");
    c.remove(f, "https://netflix.com");
    c.remove(f, "https://mit.edu");
    c.remove(f, "https://united.com");
    c.remove(f, "https://battle.net");
    c.remove(f, "https://bsky.app");
    c.remove(f, "https://cisco.com");
    c.remove(f, "https://github.com");
    c.remove(f, "https://stanford.edu");
    c.remove(f, "https://jalopnik.com");
    cout << "\nAnd, just for grins, here's the table after we've removed a few items, just to show that removal works:" << endl;
    c.printFilter(f);
    return 0;
}
