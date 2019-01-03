OBJECTS = main.o TextEditor.o
HEADERS = TextEditor.h
MyEditor: $(OBJECTS)
	g++ $^ -o myEditor
%.o: %.cpp $(HEADERS)
	g++ -c $< -o $@
clean:
	rm -f *.o myEditor
