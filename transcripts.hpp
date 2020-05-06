#ifndef TRANSCRIPTS_H
#define TRANSCRIPTS_H

#include <iostream>
#include <string>
#include <exception>
#include <vector>
//#include <thread>

#include "libxl.h"

using namespace std;
using namespace libxl ;

struct credits {
  int US ;
  int ECTS ;
} ;

typedef struct credits Credits ;

struct course {
  string code ;
  string name ;
  Credits credits ;
  Credits earnedCredits ;
  string letterGrade ;
  float points ;
} ;

typedef struct course Course ;

class Term {
  int number_of_courses ;
public:
  string name ;
  vector <Course> courses ;
  void fillCourses(int number_of_courses , Course * array) {
    this->number_of_courses = number_of_courses ;
    for(int i = 0 ; i < number_of_courses ; i++){
      courses.push_back(array[i]) ;
    }
  }
  void display_term() {
    cout << "--------------  Term : "+ name  + "\n Courses :"<< endl ;
    string coursesstr = "" ;
    for(int i = 0 ; i < this->number_of_courses ; i++){
    coursesstr += "\ncourse code :" + courses[i].code + "\ncourse name: " + courses[i].name +
    "\ncredits US: " +  to_string(courses[i].credits.US) + "\ncourse letter grade: "
    + courses[i].letterGrade + "\ncourse point grade: " + to_string(courses[i].points) + "\n\n" ;
  }
    cout << coursesstr ;
    cout << "Term Credits(US then ECTS): " +
    to_string(credits.US) + " --- " + to_string(credits.ECTS) + "\nEarned Credits(US then ECT$) : " +
    to_string(earnedCredits.US) + " --- " + to_string(earnedCredits.ECTS) + "\n Term GPA : " + to_string(gpa) + "\n\n"  ;
  }
  Credits credits ;
  Credits earnedCredits ;
  float gpa ;
  string toString(){
    string coursesstr = "" ;
    for(int i = 0 ; i < this->number_of_courses ; i++){
    coursesstr += "\ncourse code :" + courses[i].code + "\ncourse name: " + courses[i].name +
    "\ncredits US: " +  to_string(courses[i].credits.US) + "\ncourse letter grade: "
    + courses[i].letterGrade + "\ncourse point grade: " + to_string(courses[i].points) + "\n\n" ;
  }
    return "--------------  Term : "+ name  + "\n Courses :\n" + coursesstr ;
  }
} ;


struct Cumulative {
  Credits credits ;
  Credits earnedCredits ;
  float gpa ;
};
typedef struct Cumulative Cumulative ;


class Student {
public:
  string FirstName ;
  string LastName ;
  string DateOfBirth ;
  string CountryOfBirth ;
  string CIN ;
  string ID ;
  string toString() {
    return "first name : " + this->FirstName + "\n"
    +"last name : " +  this->LastName + "\nbirthday: " + DateOfBirth
    + "\ncountry : " + CountryOfBirth + "\ncin : " + CIN + "\nid : "+ ID ;
  }
};


void getStudentDetails(Book * book ,Sheet * sheet , Student * student);
void getTermDetails(Sheet * sheet  , Term * term , int * lastPos );
Cumulative getCumulative(int pos , Sheet * sheet) ;
int termsNumber(Sheet * sheet) ;



#endif
