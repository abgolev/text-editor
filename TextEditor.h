#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <cstdlib>
#include <iostream>

using namespace std;

class TextEditor;  // needed for friend line

class linkedTextLine
{
  private:
    string content;//content of the line
    linkedTextLine* nextLine;//the next line
    int lineNumber;//the line number

  public:
    linkedTextLine(string, int, linkedTextLine*);//creates a new linkedTextLine
    string getContent();//returns content
    linkedTextLine* getNextLine();//returns next line
    linkedTextLine* getNextLineMain();//used for save function in main
    int getLineNumber();//returns line number
    friend class TextEditor;  // gives TextEditor access to content, nextLine, lineNumber
};

class TextEditor
{
  private:
    linkedTextLine* head;//first line of the TextEditor

  public:
    TextEditor();
    linkedTextLine* getHead();//returns header of the text editor
    int getLength();//returns length of TextEditor
    void insertLine(string, int);//inserts a string into the list
    void deleteLine(int);//deletes a line from the list
    void printList();//prints the list
};

#endif  /* TEXTEDITOR_H */
