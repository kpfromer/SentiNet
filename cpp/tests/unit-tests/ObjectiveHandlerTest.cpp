/**
 *  @file ObjectiveHandlerTest
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 20:28:17 MDT
 *  @bug No Known Bugs
 */

#include "kernelOPS/core/ObjectiveHandler.hpp"


using namespace obps::core;

typedef struct
{
	int i;
	int j;
	int f;
} example1;

typedef struct
{
	std::string i;
	std::string j;
	double f;
} example2;

int main()
{	
	ObjectiveHandler* a = new ObjectiveHandler();
	a->add_objective(new ActionObjective(sizeof(example1)), "henry");
	a->add_objective(new ActionObjective(sizeof(example2)), "charlie");

	example1 e1 = {1, 2, 3};
	example2 e2 = {"yo", "my", 4.5};

	a->get_objective("henry")->write<example1>(&e1);
	a->get_objective("charlie")->write<example2>(&e2);

	int i = a->get_objective("henry")->read<example1>().i;
	int j = a->get_objective("henry")->read<example1>().j;
	int f = a->get_objective("henry")->read<example1>().f;

	example2 e3 = a->get_objective("charlie")->read<example2>();

	std::cout<<i<<" "<<j<<" "<<f<<std::endl;
	std::cout<<e3.i<<" "<<e3.j<<" "<<e3.f<<std::endl;

	e1 = {4, 5, 6};
	e2 = {"hello", "my", 7.1};

	a->get_objective("henry")->write<example1>(&e1);
	a->get_objective("charlie")->write<example2>(&e2);

	i = a->get_objective("henry")->read<example1>().i;
	j = a->get_objective("henry")->read<example1>().j;
	f = a->get_objective("henry")->read<example1>().f;

	e3 = a->get_objective("charlie")->read<example2>();

	std::cout<<i<<" "<<j<<" "<<f<<std::endl;
	std::cout<<e3.i<<" "<<e3.j<<" "<<e3.f<<std::endl;

	e1 = {9, 10, 11};
	e2 = {"helloldf", "mysdfkn", 90.1};


	i = a->get_objective("henry")->read<example1>().i;
	j = a->get_objective("henry")->read<example1>().j;
	f = a->get_objective("henry")->read<example1>().f;

	e3 = a->get_objective("charlie")->read<example2>();

	std::cout<<i<<" "<<j<<" "<<f<<std::endl;
	std::cout<<e3.i<<" "<<e3.j<<" "<<e3.f<<std::endl;

	return 1;
}
