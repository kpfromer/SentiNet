

#include "person.pb.h"
#include <iostream>
#include <fstream>


int main()
{

	tutorial::Person* person;
	int id = 5;
	person->set_id(id);

	std::string name("frank");
	person->set_name(name);

	std::string email("this@gmail.com");
	person->set_email(email);

	tutorial::Person::PhoneNumber* phone_number = person->add_phones();
	
	std::string number("3034781391");
	phone_number->set_number(number);
	phone_number->set_type(tutorial::Person::MOBILE);
	
	tutorial::Person::PhoneNumber* phone_number2 = person->add_phones();
	std::string number2("3030303303");
	phone_number2->set_number(number2);
	phone_number2->set_type(tutorial::Person::HOME);

}




