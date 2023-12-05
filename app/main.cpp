#include <iostream>
#include "../code/Cuckoo.h"

using namespace std;

int main(){
    Cuckoo c;
    auto filter = c.initFilter(4, 500);
    c.insert(filter, "https://www.google.com");
    if (c.contains(filter, "https://www.google.com")) {
        cout << "Filter has the URL!" << endl;
    } else {
        cout << "OHNO!" << endl;
    }

    if (c.contains(filter, "FAKEY FAKEY FAKE")) {
        cout << "Filter is bad at contains()!" << endl;
    } else {
        cout << "Filter is capable of returning false when we don't contain a URL!" << endl;
    }

    c.remove(filter, "https://www.google.com");
    if (c.contains(filter, "https://www.google.com")) {
        cout << "OHNO!" << endl;
    } else {
        cout << "Filter remove() seems to work for simple cases!" << endl;
    }

    return 0;
}
