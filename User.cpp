
#include "User.h"


    // Constructor

User::User(std::string userName_, std::string password_){

    //Grades * grade_ = new Grades[100];
    grades = new Grades[100];
    //courses = new Course[100];
    userName = userName_;
    password = password_;
    gradeCounter = 0;
}





// Getters and Setters
std::string User::getUsername() {return userName; };
std::string User::getPassword() {return password; }; 
     
void User::setName(std::string userName_) {userName = userName_; }
void User::setPassword(std::string password_) {password = password_; }

void User::addNewGrade(std::string className,int grade){
    Grades tempGerade(className,grade);
    grades[gradeCounter] = tempGerade;
    ++gradeCounter;
}


void User::deleteGrade(std::string className){
    
    int placeHolder = 0;
    for(int i = 0; i<gradeCounter;i++){
        if((grades[i].getClassName()) == className){
            placeHolder = i;
            break;
        }
    }

    for (int i = placeHolder; i < gradeCounter-1; i++)
    {   
        grades[i] = grades[i+1];
    }
    gradeCounter = gradeCounter-1;

}

void User::setEmail(std::string email_){email = email_;}



