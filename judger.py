import binding.exjudger as exjudger
import hashlib
import json
import os
import shutil
import uuid
from flask import Flask, request, Response

app = Flask(__name__)

class JudgeServer:
    @classmethod
    def ping(cls):
        data = server_info()
        data["action"] = "pong"
        return data
    @classmethod
    def exjudge(cls, src, indentSize, leftBigPara):
        result = exjudger.format_judge(src, indentSize, leftBigPara)
        ret = {"pass":result=="success", "info":result}
        return ret
    
    @classmethod
    def test(cls, src, config):
        ex_judger = exjudger.ExtraJudger(src)
        test_cases = []
        if "format" in config:
            if config["format"]["enable"]:
                indentSize = config["format"]["indentSize"]
                leftBigPara = config["format"]["leftBigPara"]
                # result = exjudger.format_judge(src, indentSize, leftBigPara)
                ex_judger.FormatJudge(indentSize, leftBigPara)
                result = ex_judger.GetResult()
                test_cases.append({"name":"format", "pass":result=="success", "info":result})
                print(result)
        if "function" in config:
            if config["function"]["enable"]:
                funclist = config["function"]["funclist"]
                funcs = ""
                for func in funclist:
                    funcs += " " + func
                ex_judger.FuncJudge(funcs)
                result = ex_judger.GetResult()
                print(result)
                test_cases.append({"name":"format", "pass":result=="success", "info":result})
        return test_cases

@app.route('/', defaults={'path': ''})
@app.route('/<path:path>', methods=["POST"])
def server(path):
    if path in ("exjudge", "ping", "test"):
        try:
            data = request.json
        except Exception:
            data = {}
        ret = {"err": None, "data": getattr(JudgeServer, path)(**data)}
    else:
        ret = {"err": "InvalidRequest", "data": "404"}
    return Response(json.dumps(ret), mimetype='application/json')

if __name__ == "__main__":
    app.run(debug=True)