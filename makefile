main:
	g++ main.cpp src/ExtraJudger.cpp src/Exception/JudgerException.cpp \
	src/JudgerFSM.cpp src/FormatJudger.cpp src/FunctionJudger.cpp src/MemoryJudger.cpp -o main
test:
	g++ test.cpp -o test