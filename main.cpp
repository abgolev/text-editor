/*
Author: Atanas Golev
Project: Text Editor
Course: CS216-003
Purpose: 
	This program reads an input from a text file. 
	The user can then choose to insert, delete, print, copy, or past. 
	The file can be saved once the user is done editing.
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include "TextEditor.h"

using namespace std;

TextEditor originalText;	//input file contents
fstream out_file;		//output file
linkedTextLine *currPointer; 	//used for save function

int newLineNumber()
{
	int newLn;
	cin >> newLn;
	return newLn;
}

string readNewEntry()//reads string that user wants to insert
{
	string newEntry;
	cout <<  "Your Input> ";
	getline(cin, newEntry);
	return newEntry;
}

string convertInt(int number)//used to convert an integer to a string
{
	stringstream ss;
	ss << number;
	return ss.str();
}

void saveFile(char *fName){
	out_file.open(fName,ios::out);//reads output file
	currPointer = originalText.getHead();//same process as in save function
        if (currPointer == NULL)
        	out_file << "";
        while (currPointer != NULL)
        {
		out_file <<  currPointer->getContent() << "\n";
     		currPointer = currPointer->getNextLineMain();
       	}
	out_file.close();//closes file
}

void printHelp(){
	cout << "Here are your options:" << endl;
	cout<<"-------------------"<<endl;
	cout<<"I: Insert at end of file"<<endl;
      	cout<<"I #: Insert at a line number" << endl;
     	cout<<"D #: Delete this line number" << endl;
      	cout<<"L: Print all lines in the file" << endl;
      	cout<<"H: Help" << endl;
      	cout<<"C #: copy this line number"<<endl;
      	cout<<"P #: paste this line number"<<endl;
     	cout<<"U: Undo"<<endl;
      	cout<<"R: Redo"<<endl;
      	cout<<"S: Save"<<endl;
      	cout<<"Q: Quit"<<endl;
      	cout<<"--------------------"<<endl;
	return;
}

int main(int argc, char** argv)
{

	const int ARGNUM = 2;

	if(argc != ARGNUM){
		cout << "Error: Did not enter a filename for the editor to open." <<endl;
		return 1;
	}

	fstream in_file;	//creates a dual input/output file

	in_file.open(argv[1]);
	if (!in_file.good())
	{
		cout << "Failure opening file: " <<argv[1] <<endl;
		return 2;
	}

	//insert and delete
	int lineNumber;				
	string userInput; 			//insert
	string lineNumberString=""; 		//user line number is first read as a string in the variable, then converted to an int
	int numCounter = 0;			//used for insert special case
	string specialcasestring="";		//used for insert special case

	//user input
 	char choice;		//single Char command
	string choiceString; 	//the whole cin String
	string fileName; 	
	int counter = 0;
	bool badInput = false;	//Makes sure the input does not any bad things
	
	//save
	string saveChoice = "";			

	//copy
	linkedTextLine *copyPointer=NULL;	

	//undo and redo
	stack<string> StackyMcStackface;	//done I D and P actions, accessed by Undo
	stack<string> undo;			//actions that have been undone, accessed by Redo
	string stackystring="";		
	string stackycontent="";
	string checky="";	
	int count = 1;

	if(!in_file.fail()){
		while (!in_file.eof())//compiles the input from the input file
   		{
      			getline(in_file, fileName);
            		originalText.insertLine(fileName, counter);
	    		counter++;
    		}
	}

	originalText.deleteLine(counter-1);	//the list enters the last line from the file twice; this deletes the copy
	in_file.close();			//closes input file

	cout << "The contents of the text file: "<< endl<<endl;
   	originalText.printList();	//prints the list compiled from input
	printHelp();			//prints the user options

   	do
   	{
        	cout<<"Please enter your choice here: ";
		getline(cin, choiceString);
		choice = choiceString[0];

	 	switch(choice){

        		case 'I'://insert

				lineNumberString="";	//used for the Line Number the user input
				if(choiceString=="I")	//If the user just types 'I', inserts it into the last line
				{
					userInput = readNewEntry();					//read user input
					stackycontent=userInput;					//used for Undo stack
					stackystring = "I" + convertInt(originalText.getLength()+2);	//adds to undo stack
					StackyMcStackface.push(stackycontent);				//adds to undo stack
					StackyMcStackface.push(stackystring);				//adds to undo stack
					originalText.insertLine(userInput,originalText.getLength()+1);	//inserts line into original list
				}
				else if(choiceString[1]!=' ')	//handles exceptions
					cout<<"Invalid command; second character of input must be a space."<<endl;
				else	//if user has input I #
				{
					for(int i=2; i<=choiceString.length()-1;i++)//reads the # part
					{
						//if(isdigit(choiceString[i]))//makes sure the input is a valid int
						if(choiceString[i])//makes sure the input is a valid int
							lineNumberString=lineNumberString+choiceString[i];//adds every valid int onto LineNumberString
						
						else//if it is not a valid string
						{
							cout<<"You did not input a valid integer for the line number!"<<endl;
							badInput=true;//makes sure it doesn't enter the next step
							break;
						}
					}//end for

					if(atoi(lineNumberString.c_str())==0)
						cout<<"You did not input a valid integer for the line number!"<<endl;
					else if(!badInput)//if the input is valid
					{
				    		lineNumber = atoi(lineNumberString.c_str());//converts line number into an It
	 			    		userInput = readNewEntry();//reads the string the user wants to insert
        	    				stackycontent = userInput;//used for undo stack
						stackystring = "I"+lineNumberString; //used for undo stack
						originalText.insertLine(userInput, lineNumber-1 );//inserts line into the list
						StackyMcStackface.push(stackycontent);
						StackyMcStackface.push(stackystring);
						while(!undo.empty())
							undo.pop();	//pops off everything in Redo stack
					}//end if
				}//end else
				badInput=false;//resets exception handling
  	         		break;

  		       case 'D'://if user chooses to delete
				lineNumberString = "";
				copyPointer=originalText.getHead();//used for Undo function

				if(choiceString=="d")//makes sure entry is valid
					cout<<"Invalid entry; must include a line number as input as well."<<endl;
				else if(choiceString[1]!=' ')//makes sure entry is valid
					cout<<"Invalid; second character must be space in input."<<endl;
				else
				{
					for(int i=2; i<=choiceString.length()-1;i++)//reads the int the user has input
					{
						if(isdigit(choiceString[i]))
							lineNumberString=lineNumberString+choiceString[i];
						else//if integer is not valid
						{
							cout<<"You did not input a valid int for the line number!"<<endl;
							badInput=true;
							break;
						}
					}
				}

				if(!badInput)
				{
 			    		lineNumber = atoi(lineNumberString.c_str());//reads the line user wants to delete

					for(int i=0; i<lineNumber-1; i++)//used for the Undo/Redo functions
					{
						if(copyPointer==NULL)
							break;
						else
							copyPointer = copyPointer -> getNextLineMain();	//finds appropriate line for deletion
					}

					if(copyPointer!=NULL){		//if valid integer
						stackystring = "D"+lineNumberString;			//pushes onto stack
						StackyMcStackface.push(copyPointer->getContent());	//pushes deleted content into stack
						StackyMcStackface.push(stackystring);
						while(!undo.empty())
							undo.pop();	//clears everything in undone stack 
					}

         		   		originalText.deleteLine(lineNumber-1);//deletes the line
				}
				badInput=false;
				break;

        		 case 'L'://prints the list
				if(choiceString.length()>1){
					cout<<"Invalid command; try again."<<endl;
					break;
				}//checks if input is valid

		            	cout<<endl<<"Here is your file: "<<endl;
		            	originalText.printList();
				cout<<endl;
		            	break;

			 case 'H'://help menu
				if(choiceString.length()>1){
					cout<<"Invalid command; try again."<<endl;
					break;
				}//checks if input is valid
				printHelp();
	   		 	break;

			 case 'C'://copy
				lineNumberString="";

				if(choiceString=="C"||choiceString[1]!=' '){//makes sure the input is valid
					cout<<"Invalid format for Copy; please try again."<<endl;
					break;
				}
				else
				{
					for(int i=2; i<=choiceString.length()-1;i++)//Looks for which line needs to be copied
					{
						if(isdigit(choiceString[i]))
							lineNumberString=lineNumberString+choiceString[i];
						else//If the line to be copied is not a part of the list
						{
							cout<<"You did not input a valid int for line number!"<<endl;
							badInput=true;
							break;
						}
					}//end for
				}//end else

				lineNumber = atoi(lineNumberString.c_str());//reads the line user wants to copy
				if(lineNumber==0)
					cout<<"Sorry; invalid line number to copy!"<<endl;
				else if(!badInput)//if the input line is valid
				{
					copyPointer = originalText.getHead();
					for(int i=0; i<lineNumber-1; i++){//goes through the list until the appropriate line is found
						if(copyPointer==NULL)
							break;
						else
							copyPointer = copyPointer->getNextLineMain();
					}
		
					if(copyPointer==NULL)//if the line is not valid
						cout<<"Sorry; invalid line number to copy!"<<endl;
				}

				badInput=false;
				break;

			 case 'P'://paste
				lineNumberString="";
				if(choiceString=="P"||choiceString[1]!=' ')//makes sure input is valid
					{cout<<"Invalid format for P entry; try again."<<endl;break;}
				else if(copyPointer==NULL)//makes sure something has been copied
					{cout<<"There is nothing copied to the clipboard; sorry!"<<endl;break;}
				else
				{
					for(int i=2; i<=choiceString.length()-1;i++)//looks for the line in which to paste
					{
					        if(isdigit(choiceString[i]))
		                                	lineNumberString=lineNumberString+choiceString[i];
		                                else//if the input is not an integer
		                                {
							cout<<"You did not input a valid int for line number!"<<endl;
							badInput=true;
							break;
						}
					}//end for
				}//end else

				lineNumber = atoi(lineNumberString.c_str());//reads the line user wants to copy
				if(lineNumber==0)
					cout<<"Sorry; invalid line number to paste!"<<endl;
				else if(!badInput)//if the input line is valid
				{
					originalText.insertLine(copyPointer->getContent(), lineNumber-1);//inserts into appropriate line
					stackystring = "I"+lineNumberString;//inserts into the undo stack
		                        StackyMcStackface.push(copyPointer->getContent());
		                        StackyMcStackface.push(stackystring);
					while(!undo.empty())
						undo.pop();//undoes everything in undo stack
				}//end if

				badInput=false;//resets exception handling
				break;

			 case 'U'://if user chooses to Undo
				lineNumberString="";
				count=1;//used to read the Undo stack
				numCounter=0;
				checky="";
				if(choiceString.length()>1)
					{cout<<"Invalid command; try again."<<endl;break;}//makes sure that input is valid
		
				if(StackyMcStackface.empty())
					{cout<<"There is nothing to undo."<<endl;break;}//if the stack is empty
				else
				{
					checky=StackyMcStackface.top();//checks whether last action was Insert or Delete action

					while(isdigit(checky[count]))//reads the line number
					{
						lineNumberString = lineNumberString + checky[count];
						count++;
					}

					StackyMcStackface.pop();//pops off Insert/Delete command
					userInput = StackyMcStackface.top();//the next node is the UserInput
					undo.push(StackyMcStackface.top());//inserts into the Redo Node
					StackyMcStackface.pop();
					undo.push(checky);//inserts the Insert/Delete command into the Redo node
					lineNumber=atoi(lineNumberString.c_str());//Converts Line Number into integer

					if(checky[0]=='D')//if it's a delete type action, inserts it back
						originalText.insertLine(userInput, lineNumber-1);//Inserts
					else//Same, but in this case, deletes what was added by insert type action
						originalText.deleteLine(lineNumber-1);
				}//end else
				break;

			 case 'R'://Redo
				lineNumberString="";
				count=1;
				checky="";
				if(choiceString.length()>1){cout<<"Invalid command; try again."<<endl;break;}	//checks for valid input
				if(undo.empty()){cout<<"Nothing has been undone so there is nothing to redo; sorry!"<<endl;break;}	//checks for valid input
				else
				{
					checky=undo.top();//checks top spot to see if it's a delete or insert type action
					while(isdigit(checky[count]))//reads the line number
					{
		                        	lineNumberString = lineNumberString + checky[count];
		                        	count++;
					}

		                        undo.pop();
        		                userInput = undo.top();
        		                StackyMcStackface.push(undo.top());
        		                undo.pop();
        		                StackyMcStackface.push(checky);
					lineNumber=atoi(lineNumberString.c_str());

					if(checky[0]=='D')//if it's a delete action, reinserts it
						originalText.deleteLine(lineNumber-1);
					else//if it's an insert action, redeletes it
						originalText.insertLine(userInput, lineNumber-1);
				}//end else
				break;

			 case 'S'://saving the file
				if(choiceString.length()>1)
					{cout<<"Invalid command; try again."<<endl;break;}
				saveFile(argv[1]);
				break;

		         case 'Q'://quits the file, after asking user to save or not
				if(choiceString.length()>1)
					{cout<<"Invalid command; try again."<<endl;break;}

		   		while(saveChoice!="Y" && saveChoice!="y" && saveChoice!="N" && saveChoice!="n")
	    			{
		 			cout << "Do you want to save the changes? (Y - yes; N - no): ";
					cin >> saveChoice;//saves choice user makes

					if(saveChoice=="Y" || saveChoice == "y")
					{
						saveFile(argv[1]);	//specifies output file
				        }//end if
			 	}//end while
           		    	break;

        		 default://if user selects an invalid choice
           			 cout<<"\nInvalid choice. Please try again. Type H for help.\n";
            			 break;
     		 }//end choice
   	} while(choice != 'Q');	//end do
	out_file.close();
	cout<< "Byebye"<<endl; 
	return 0;
}//end main

