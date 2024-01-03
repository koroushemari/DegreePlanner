#include "Grades.h"

Grades::Grades(){
    className = "";
    grade = 0;
    
}

Grades::Grades(std::string className_, int grade_){
    className = className_;
    grade = grade_;
}