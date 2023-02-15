import requests

session = requests.session()
data1 = {
    "problem_id": 1,
    "language": "C++",
    "code":"#include <bits/stdc++.h>\n\nusing namespace std;\n\nint main()\n{\n\tlong long a,b;\n  cin >> a >> b;\n  cout << a+b;\n}",
    "extra_config": {
        "format": {
            "enable": True,
            "indent_size": 4,
            "left_big_para": True
        }
    },
    "extra_option": True
}

headers = {
    "Cookie": "sessionid=ldssubg7ahyfo0rshrmppvf0actu5gmn"
}

url = "http://101.43.198.81/api/"

data2 = {
    "username": "123",
    "password": "123456"
}


""" resp = session.post(url+"login",data=data2)
print(resp.text)
print(resp.cookies) """
resp = session.post(url+"submission",data=data1, headers=headers)
print(resp.text)
print(resp.cookies)