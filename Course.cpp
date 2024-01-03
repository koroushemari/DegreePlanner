#include "Course.h"
#include <iostream>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>

#include <Wt/WTimer.h>

#include <Wt/WAnchor.h>
#include <Wt/WMenu.h>
#include <Wt/WMessageBox.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WTable.h>

#include <Wt/WText.h>

#include "Degreeplanner.h"
#include "Course.h"
#include "Module.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cctype>
#include <Wt/WBootstrapTheme.h>

#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WLineEdit.h>
#include <Wt/WText.h>

#include <Wt/WIconPair.h>
#include <Wt/WTree.h>
#include <Wt/WTreeTable.h>
#include <Wt/WTreeTableNode.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WAnchor.h>
#include <Wt/WApplication.h>

#include <Wt/WServer.h>


Course::Course(){
    
}

Course::Course(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText): className(className_), classYear(classYear_),description(description_){
    //courseButton = courseContainer -> addNew<Wt::WPushButton>(className_);
    preReq = new std::string[100];
    preReqCounter = 0;

}


void Course::setButtonStyle(std::string css_){
    courseButton->setStyleClass(css_);
   
}

void Course::setCourseButton(Wt::WPushButton *button){
    courseButton = button;
}

void Course::setButtonCss(std::string css_){
    courseButton ->setAttributeValue("style",css_);
}


Wt::WPushButton* Course::getClassButton(){

    return courseButton;
}



void Course::buttonClicked(Wt::WText* descriptionText){

    courseButton->clicked().connect([=] {
        descriptionText -> setText(description);
    });

}

void Course::addPreReq(std::string courseName){
    preReq[preReqCounter] = courseName;
    ++preReqCounter;
}

void Course::setxy(int x_, int y_){
    this -> x = x_;
    this -> y = y_;
}

std::string Course::getPreReq(int index){
    return preReq[index];
}

void Course::setCourseButtonIndex(int index){
    courseButtonIndex = index;
}

void Course::setGraded(){
    graded = 1;
}

void Course::setUngraded(){
    graded = 0;
}


int Course::getGraded(){
    
    if((graded != 0)&&(graded != 1)){
        graded = 0;
    }
    return graded;
    }
//get the class name


