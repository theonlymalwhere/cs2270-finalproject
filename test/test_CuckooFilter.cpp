// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <ctime>
#include <cmath>

// in order to access private members, we have to defeat the safety
// mechanisms. This is acceptable for unit testing, but not much
// else. It is also not portable because it is not supported by the
// C++ spec, and therefore might not work with all compilers.
#define private public
#include "../src/Cuckoo.h"

using namespace std;

class test_Cuckoo : public ::testing::Test {
protected:
	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {}
};

double test_Cuckoo::total_grade = 0;
double test_Cuckoo::max_grade = 144;


// forward declaration of helpers. read and learn from them!
shared_ptr<Graph> buildGraphToClear();
shared_ptr<Graph> mkgraph();
shared_ptr<Node> find(shared_ptr<Graph> graph, string label);

// Unit Tests

TEST_F(test_Cuckoo, Cuckoo_init){
  shared_ptr<Node> a(new Node("a"));
  shared_ptr<Node> b(new Node("b"));
  shared_ptr<Edge> ab(new Edge(a, b));
  ab->type = CROSS_EDGE;
  ab->setType(FORWARD_EDGE);
  ASSERT_EQ(ab->type, FORWARD_EDGE);
  add_points_to_grade(5);
  ab->setType(UNDISCOVERED_EDGE);
  ASSERT_EQ(ab->type, UNDISCOVERED_EDGE);
  add_points_to_grade(5);
}