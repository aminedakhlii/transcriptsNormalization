#include <chrono>
#include <fstream>


#include "transcripts.hpp"

int termsNum ;
Cumulative cumulative ;
int sheetNumber ;
char * file ; 


void generateStudentXML(Student student) {

  fstream xml ;
  xml.open("transcript.xml" , ios::out) ;
  xml <<"<transcript ml-update=""aware"" ml-stage=""preload"" ml-view=""top"">\n"
  "<Student>\n"
  "<firstname>" << student.FirstName << "</firstname>\n"
  "<lastname>" << student.LastName << "</lastname>\n"
  "<dateofbirth>" << student.DateOfBirth << "</dateofbirth>\n"
  "<countryofbirth>" << student.CountryOfBirth << "</countryofbirth>\n"
  "<cin>" << student.CIN << "</cin>\n"
  "<id>" << student.ID << "</id>\n"
  "</Student>\n";
  xml.close() ;

}

void generateTermsXml(vector<Term> terms){

  fstream xml;
  xml.open("transcript.xml" , ios::app) ;
  for( Term t : terms){
  xml<< "<term>\n"
  "<name>\n"<< t.name <<"\n</name>\n";

  for(auto c : t.courses){
  xml <<
  "<course>"
  "<code>"<< c.code <<"</code>" << endl <<
  "<name>"<< c.name <<"</name>"<< endl <<
  "<credits>" << endl <<
  "<us>"<< c.credits.US <<"</us>" << endl <<
  "<ects>"<< c.credits.ECTS <<"</ects>" << endl <<
  "</credits>" << endl <<
  "<earnedcredits>" << endl <<
  "<us>"<< c.earnedCredits.US <<"</us>" << endl <<
  "<ects>"<< c.earnedCredits.ECTS <<"</ects>" << endl <<
  "</earnedcredits>" << endl <<
  "<garade>" << endl <<
  "<letter>"<< c.letterGrade <<"</letter>" << endl <<
  "<points>"<< c.points <<"</points>" << endl <<
  "</garade>" << endl <<
  "</course>" ;

}


  xml <<
  "<credits>"<< endl <<
  "<!-- Total credits in this term -->"<< endl <<
  "<us>"<< t.credits.US <<"</us>"<< endl <<
  "<ects>"<< t.credits.ECTS <<"</ects>"<< endl <<
  "</credits>"<< endl <<
  "<earnedcredits>\n<!-- Total earned credits in this term -->"<< endl <<
  "<us>"<< t.earnedCredits.US <<"</us>"<< endl <<
  "<ects>"<< t.earnedCredits.ECTS <<"</ects>"<< endl <<
  "</earnedcredits>"<< endl <<
  "<gpa>"<< t.gpa << "</gpa>"<< endl <<
  "</term>" ;
}
  xml.close();

}


void generateCumulativeXml(Cumulative cumu) {

  fstream xml ;
  xml.open("transcript.xml", ios::app) ;
  xml << "<cumulative>" << endl <<
  "<credits>\n<!-- Cumulative attempted credits -->" << endl <<
  "<us>" << cumu.credits.US << "</us>" << endl <<
  "<ects>" << cumu.credits.ECTS << "</ects>" << endl <<
  "</credits>" << endl <<
  "<earnedcredits>\n<!-- Cumulative earned credits -->" << endl <<
  "<us>" << cumu.earnedCredits.US << "</us>" << endl <<
  "<ects>" << cumu.earnedCredits.ECTS << "</ects>" << endl <<
  "</earnedcredits>" << endl <<
  "<gpa>" << cumu.gpa << "</gpa>" << endl <<
  "</cumulative>\n</transcript>" ;
  xml.close() ;

}


void threadFunc(int iteration ,vector<Term> * terms ,  int * lastTermPosition , bool Gcumulative){
  Book * book = xlCreateXMLBook() ;
  if(book)
    {
      if(book->load(file))
        {

          Sheet* sheet = book->getSheet(sheetNumber);
          if(sheet){


                // get the number of potential terms and store their details in a vector
                int max = (!Gcumulative) ? 3 : termsNum % 3 ;

                bool divisorOfThree = (termsNum % 3 == 0) ;


                for(int i = 0 ; i < max ; i++){
                    Term nextTerm ;
                    getTermDetails(sheet,&nextTerm,lastTermPosition);
                    terms->push_back(nextTerm) ;
                    if( divisorOfThree && iteration == termsNum/3 - 1 && i == max - 1 )
                        Gcumulative = true ;
                  }


                if(Gcumulative){
                  cumulative = getCumulative(*lastTermPosition + 2, sheet) ;
                  cout << cumulative.gpa << endl ;
                }

        }
      }
      book->release();
    }

}


void FirstAccess(int * termsNum , Student * student){
  Book * book = xlCreateXMLBook() ;
  if(book)
    {
      if(book->load(file))
        {

          Sheet* sheet = book->getSheet(sheetNumber);
          if(sheet){
                // get the number of potential terms and store their details in a vector
                  getStudentDetails(book , sheet , student) ;
                * termsNum = termsNumber(sheet) ;
        }
      }
    }
}




int main(int argc , char * argv[])
{
  auto start = chrono::high_resolution_clock::now();

  file = argv[1] ;

  Student student ;
  int lastTermPosition = 20 ;
  sheetNumber = 0;
  //sheetNumber = 1 ;

  FirstAccess(&termsNum,&student);

  //cout << termsNum / 3 << endl << termsNum % 3 << endl <<termsNum;

  vector<Term> terms ;

  for(int i=0 ; i < termsNum / 3 ; i++){
  threadFunc(i,&terms, &lastTermPosition,false);
  }


  if(termsNum % 3 != 0)
  threadFunc(-1,&terms ,&lastTermPosition,true) ;

  generateStudentXML(student) ;
  generateTermsXml(terms);
  generateCumulativeXml(cumulative);

  terms.clear() ;

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  cout << duration.count() << endl ;

    return 0;
}
