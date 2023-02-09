import binding.exjudger as judger

file = open("data/2.in")
status = judger.judge(file.read(),4,True)
print(status)