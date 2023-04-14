import binding.exjudger as exjudger

f = open("data/2.in")
code = f.read()

judger =  exjudger.ExtraJudger(code)

def Memory():
    print(judger.MemoryJudge("nums",True))
    print(judger.GetResult())

def Format():
    print(judger.FormatJudge(4,True,True,3))
    print(judger.GetResult())

def Func():
    print(judger.FuncJudge("sort", "init", 5, True))
    print(judger.GetResult())

def Style():
    print(judger.StyleJudge("global_", "ans", 3,3,3, True))
    print(judger.GetResult())
    
    
# Style()
