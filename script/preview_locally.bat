@echo off
echo Starting MkDocs server...

start http://127.0.0.1:8000/

mkdocs serve

echo Server stopped.
pause