#!/bin/bash
# our comment is here
echo "ssss"
git add .
git commit -m "commit"
git push -u origin master
expect "Username for 'https://github.com':"
send "altair200333"
