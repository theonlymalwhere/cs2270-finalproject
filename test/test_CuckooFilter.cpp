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

class test_Graph : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)0;
			outgrade.close();
		}
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
			std::cout << "Total Grade is : " << (int)std::ceil(100*total_grade/max_grade) << std::endl;
		}
	}

	void add_points_to_grade(double points){
		if(!::testing::Test::HasFailure()){
			total_grade += points;
		}
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
		}
	}
	
	static double total_grade;
	static double max_grade;
};

double test_Graph::total_grade = 0;
double test_Graph::max_grade = 144;


// forward declaration of helpers. read and learn from them!
shared_ptr<Graph> buildGraphToClear();
shared_ptr<Graph> mkgraph();
shared_ptr<Node> find(shared_ptr<Graph> graph, string label);

// Unit Tests

TEST_F(test_Graph, Graph_EdgeSetType){
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