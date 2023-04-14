main:
	g++ -std=c++11 main.cpp src/ExtraJudger.cpp src/Exception/JudgerException.cpp src/JudgerFSM.cpp \
	src/FormatJudger.cpp src/FunctionJudger.cpp src/MemoryJudger.cpp src/StyleJudger.cpp -o main 
test:
	g++ -std=c++11 test.cpp -o test