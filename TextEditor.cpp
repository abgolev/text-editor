#include <cstdlib>
#include <iostream>
#include <ctime>
#include "TextEditor.h"

using namespace std;

//Linked Text Line Members
//---------------------------------------------------

linkedTextLine::linkedTextLine(string data, int ln,  linkedTextLine *nextLn):
   content(data), lineNumber(ln),  nextLine(nextLn)
{}

//returns content of line
string linkedTextLine::getContent()
{
   return content;
}

//returns line number
int linkedTextLine::getLineNumber()
{
	return lineNumber;
}

//returns next line
linkedTextLine *linkedTextLine::getNextLine()
{
   return nextLine;
}

//returns next line; can be accessed by main for save fucntion
linkedTextLine *linkedTextLine::getNextLineMain()
{
	return nextLine;
}


//Text Editor Members
//---------------------------------------------------

TextEditor::TextEditor() :
    head (NULL)
{}

//returns first line in list
linkedTextLine * TextEditor::getHead()
{
	return head;
}

int TextEditor::getLength()
{
	int HowLong=0;
	linkedTextLine *currPtr=head;
	while(currPtr!=NULL && currPtr->nextLine!=NULL){
		HowLong=HowLong+1;
		currPtr = currPtr->nextLine;
	}
	return HowLong;
}

//inserts line into the list
void TextEditor::insertLine(string stringy, int newLineNum)
{
   linkedTextLine *newLine = new linkedTextLine("",0,NULL);
   linkedTextLine *currPtr = head;
   linkedTextLine *prevPtr = NULL;
   linkedTextLine *newTxtLn;	//points to a new Line
   int currCountNumber=0;	//used for lines larger than last line in the list
   bool specialCase = false;	//used for lines that are larger than the last line in the list

   //goes to the appropriate spot in the list based on the lineNumber
   while(currPtr != NULL && newLineNum > currPtr->lineNumber )
   {
   	prevPtr = currPtr;
   	currCountNumber = prevPtr->lineNumber + 1;
   	currPtr = currPtr->nextLine;
   }

   while(newLineNum > currCountNumber)
   {//if the line number to be inserted is larger than last line in list
	if(prevPtr == NULL){
		head=newLine;
		currCountNumber++;
		prevPtr = head;
	}
	specialCase = true;
	newTxtLn = new linkedTextLine ("", currCountNumber, currPtr);	//creates new blank line
	prevPtr->nextLine = newTxtLn;
	prevPtr=prevPtr->nextLine;
	currCountNumber++;
   }

   newTxtLn = new linkedTextLine(stringy, newLineNum, currPtr);//creates a new line to add to the list!!

   if(newLineNum==0)//make new line header if it goes into the beginning
        head = newTxtLn;
        
   if(prevPtr != NULL)//connect the previous line to the newly inserted line
        prevPtr->nextLine = newTxtLn;
        
   while(currPtr != NULL && !specialCase)//update the position numbers of each item in the list
	{
	currPtr->lineNumber = currPtr->lineNumber+1;
	currPtr = currPtr->nextLine;
	}
}//end insert function

void TextEditor::deleteLine(int lnNum)//used to delete a line
{
   linkedTextLine *delPtr = head;
   linkedTextLine *prevPtr = NULL;

   // If the list is empty, does nothing
   if (delPtr == NULL){
	cout<<"Cannot delete because the list is empty!"<<endl;
        return;
   }

   //Treat the first line as a special case: head needs to be updated
   if (lnNum==0)
   {
         head = head->nextLine;
	 delPtr=head;

         while(delPtr != NULL)
	 {
                delPtr->lineNumber = delPtr->lineNumber-1;
                delPtr = delPtr->nextLine;
         }
   }//end if
   else
   {
        prevPtr = delPtr;

	//Look for the line to be deleted
        while (delPtr != NULL && lnNum !=delPtr->lineNumber)
        {
            prevPtr = delPtr;
            delPtr = delPtr->nextLine;
        }

	//if the line is found
        if(delPtr != NULL)
	{
                prevPtr->nextLine = delPtr->nextLine;
		while(delPtr != NULL)
		{
        		delPtr->lineNumber = delPtr->lineNumber-1;
        		delPtr = delPtr->nextLine;
		}
	}

	//if the line is not found
	else
	{
		cout<<"This line does not exist."<<endl;
	}
    }//end else
}//end delete function

void TextEditor::printList ()//prints the list
{
    linkedTextLine *currPtr;
    currPtr = head;
    int count = 0;
    if (currPtr == NULL)
    {
        cout << "There is nothing in the file." << endl;
        return;
    }

    while (currPtr != NULL)
    {
        count++;
        cout << count << "> " << currPtr->content << endl;
        currPtr = currPtr->nextLine;
    }
}
