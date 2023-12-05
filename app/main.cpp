#include <iostream>
#include "../code/Cuckoo.h"

using namespace std;

int main(){
    Cuckoo c;
    auto filter = c.initFilter(500, 10, 10);
    c.insert(filter, "https://www.google.com");
    c.insert(filter, "https://www.google.com");
    c.insert(filter, "https://www.google.com");

    // These two produce identical hashes:
    // c.insert(filter, "https://www.purple.com");
    // c.insert(filter, "https://www.nilenet.com");

    // Rudimentary cuckoo check
    // c.insert(filter, "https://www.google.com");
    // c.insert(filter, "https://www.google.com");
    // c.insert(filter, "https://www.google.com");
    // c.remove(filter, "https://www.google.com");
    // c.remove(filter, "https://www.google.com");
    // c.remove(filter, "https://www.google.com");

    // if (c.contains(filter, "https://www.google.com")) {
    //     cout << "OHNO!" << endl;
    // } else {
    //     cout << "Cuckoo add and remove seems to work!" << endl;
    // }

    return 0;
}
