CXX = g++
CFLAGS = -std=c++14
LIB = dashboard.h display.h main.h sonar.h status.h ./PaintLib/font.h ./PaintLib/paint.h
OBJ = dashboard.cpp display.cpp main.cpp sonar.cpp ./PaintLib/font.o ./PaintLib/paint.o
BIN = Sonar

all: $(BIN)
	@./$(BIN)

$(BIN): $(OBJ)
	@$(CXX) $(OBJ) -o $(BIN) -luser32 -lgdi32 -lmsimg32

paint.o: $(LIB) ./PaintLib/paint.cpp
	@$(CXX) $(CFLAGS) -c ./PaintLib/paint.cpp

font.o: $(LIB) ./PaintLib/font.cpp
	@$(CXX) $(CFLAGS) -c ./PaintLib/font.cpp

sonar.o: $(LIB) sonar.cpp
	@$(CXX) $(CFLAGS) -c sonar.cpp

main.o: $(LIB) main.cpp
	@$(CXX) $(CFLAGS) -c main.cpp

display.o: $(LIB) display.cpp
	@$(CXX) $(CFLAGS) -c display.cpp

dashboard.o: $(LIB) dashboard.cpp
	@$(CXX) $(CFLAGS) -c dashboard.cpp

clean: 
	@rm *.o
	@rm ./$(BIN)
