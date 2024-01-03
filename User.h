#ifndef USER_H
#define USER_H

#include <string>
#include "Course.h"
#include "Grades.h"
#include "Module.h"

class User
{

public:
    // Constructor
    User(std::string userName, std::string password);

    // Getters and Setters
    std::string getUsername();
    std::string getPassword();
     
    void setName( std::string userName_);
    void setPassword(std::string password_);

    void addNewGrade(std::string className,int grade);

    void deleteGrade(std::string className);
    int returnGradeSize(){return gradeCounter;}
    Grades returnGradeByIndex(int i){return grades[i];}

    void setEmail(std::string email_);

private:
    // Attributes
    Grades* grades;
    std::string userName;
    std::string password;
    int gradeCounter;
    std::string email;
};

#endif // USER_H