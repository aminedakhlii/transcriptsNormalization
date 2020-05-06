#include "transcripts.hpp"

void getStudentDetails(Book * book ,Sheet * sheet , Student * student)
{
    int year, month, day;
    book->dateUnpack(sheet->readNum(9, 3), &day, &month, &year);
    student->FirstName = sheet->readStr(7, 3);
    student->LastName = sheet->readStr(8, 3);
    student->DateOfBirth = to_string(year) + "-" + to_string(month) + "-" + to_string(day) ;
    student->CountryOfBirth = sheet->readStr(10, 3);
    student->CIN = sheet->readStr(11, 3);
    student->ID = sheet->readStr(12, 3);
}


void getTermDetails(Sheet * sheet  , Term * term , int * lastPos ){

  int i = * lastPos + 1 ;
  while(i < sheet->lastRow()){
  try {
    // finding the name of the term
    string s = sheet->readStr(i,2) ;
    if(s.find("Fall") != string::npos ||
    s.find("Spring") != string::npos ||
     s.find("Summer") != string::npos ){
              // filling the term details :
              term->name = sheet->readStr(i,2);
              // must allocate dynamically
              //----------------------------- Filling courses ----------------------//
              Course  array[8] = {};
              int back = i - 1 , a = 0 ;
              while (back > 21){
                try {

                  string  potentialCourse = sheet->readStr(back,2) ;
                if(potentialCourse.find("Fall") != string::npos ||
                potentialCourse.find("Spring") != string::npos ||
                 potentialCourse.find("Summer") != string::npos )
                        break ;
                else{
                array[a].code = sheet->readStr(back,2) ;
                array[a].name = sheet->readStr(back,3) ;
                array[a].credits.US = sheet->readNum(back,4) ;
                array[a].credits.ECTS = sheet->readNum(back,5) ;
                array[a].earnedCredits.US = sheet->readNum(back,12) ;
                array[a].earnedCredits.ECTS = sheet->readNum(back,6) ;
                array[a].letterGrade = sheet->readStr(back,7) ;
                array[a].points = sheet->readNum(back,11) ;
                a++ ;
                back -- ;
              }
            }
            catch(exception &ex) {
              break ;
            }
          }

             term->fillCourses(a, array) ;

        // -------------------------------------------------------------//

        // - ---------- Credits and earned credits ----------------------//
        term->credits.US = sheet->readNum(i,4) ;
        term->credits.ECTS = sheet->readNum(i,5) ;
        term->earnedCredits.US = sheet->readNum(i,13) ;
        term->earnedCredits.ECTS = sheet->readNum(i,6) ;
        term->gpa = sheet->readNum(i+1,7);


        // -----------------------------------------------------------//
        *lastPos = i ;
        break ;
    }
    i++ ;
  }
  catch(exception &e) {
    i++ ;
  }
}
}




Cumulative getCumulative(int pos , Sheet * sheet) {
  Cumulative cu ;
  cu.credits.US = sheet->readNum(pos,4) ;
  cu.credits.ECTS = sheet->readNum(pos,5) ;
  cu.earnedCredits.US = sheet->readNum(pos+1 ,4) ;
  cu.earnedCredits.ECTS = sheet->readNum(pos+1,5) ;
  cu.gpa = sheet->readNum(pos+2,7) ;
  return cu ;
}




int termsNumber(Sheet * sheet){
  int number = 0 ;
  int i = 21 ;
  while(i < 100){
  try {
    // finding the name of the term
    string s = sheet->readStr(i,2) ;
    if(s.find("Fall") != string::npos ||
    s.find("Spring") != string::npos ||
     s.find("Summer") != string::npos ){

        number += 1 ;
     }
     i++ ;
   }
   catch(exception &e) {
     i++;
   }
 }
 return number ;
}
