# py_to_exe
An app that converts a python script into an executable:

1.: gets input(path of .py script)
2.: stores the input conde into a string
3.: opens a new c file
4.: write a template c code into the new c file and a string variable that stores the embedded python code
5.: compiles the new c file , then deletes it
      |
      -> output:               
          executable: (when executed)
              1.opens a new python file
              2.writes the embedded python script into the new python file
              3.runs the new python file with python3 command
              4.deletes the python file
