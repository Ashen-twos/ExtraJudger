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
        result = exjudger.judge(src, indentSize, leftBigPara)
        ret = {"pass":result=="success", "info":result}
        return ret


@app.route('/', defaults={'path': ''})
@app.route('/<path:path>', methods=["POST"])
def server(path):
    if path in ("exjudge", "ping"):
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