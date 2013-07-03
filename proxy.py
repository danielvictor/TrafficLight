#!/usr/bin/env python

import requests
from requests.auth import HTTPBasicAuth
from flask import Flask

app = Flask(__name__)

URL = "http://jenkins.example.com/job/<your-project-name>/lastBuild/api/json"
CREDS = ("username", "password")

def get_status():
    """Get the build status from Jenkins
    """
    r = requests.get(URL, auth=HTTPBasicAuth(*CREDS))
    return r.json


def parse_status(status):
    """Parse the status returned by Jenkins
    """
    r = status.get('result')
    if r == "SUCCESS":
        return "G"
    elif r == "FAILURE":
        return "R"
    else:
        return "O"


@app.route("/")
def index():
    """Serve the index page
    """
    return ">" + parse_status(get_status())


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=1983)
