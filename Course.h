#ifndef COURSE_H
#define COURSE_H
#include <iostream>
#include <string>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>


class Course{

public:
    //constructor
    
    Course();
    Course(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText);
    //get the class name
    std::string getClassName(){ return className;}
    
    // get the class year
    int getClassYear(){ return classYear;}

    int getX(){ return x;}
    int getY(){ return y;}

    void setCourseButton(Wt::WPushButton *button);
    //get the class description
    std::string getDescription(){ return description;}

    void setButtonStyle(std::string css_);

    void setButtonCss(std::string css_);

    Wt::WPushButton* getClassButton();

    void buttonClicked(Wt::WText* descriptionText);

    void setxy(int x_, int y_);

    void addPreReq(std::string className);

    int getPreReqSize(){ return preReqCounter;}

    std::string getPreReq(int index);

    int getButtonIndex(){
        return courseButtonIndex;
    }

    void setCourseButtonIndex(int index);

    void setGraded();
    void setUngraded();

    int getGraded();


//attributes 
private: 
    std::string className;
    int classYear;
    std::string description;
    Wt::WPushButton *courseButton;
    std::string* preReq;
    int x;
    int y;
    int preReqCounter;
    int courseButtonIndex;
    int graded;

};
#endif // COURSE_H