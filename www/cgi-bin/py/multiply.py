#!/usr/bin/env python3

import cgi, cgitb
cgitb.enable()

input_data = cgi.FieldStorage()

print('Content-Type: text/html') # HTML is following
print('')                        # Leave a blank line
print('<body style="width:100%; font-family: sans-serif, standard, courier, arial, helvética; background-color: rgb(229, 220, 245); margin-top:30px">')
print('<div style="width:400px; margin:0 auto; text-align: center; background-color: rgb(180, 116, 228); border-radius: 10px; padding: 10px; height:260px;">')
print('<h1>Multiply Results</h1>')
try:
    num1 = int(input_data["num1"].value)
    num2 = int(input_data["num2"].value)
except:
    print('<output>Sorry, the script cannot turn your inputs into numbers (integers).</output>')
    print('<p><br/><br/><a href="../../index.html">Back to homepage</a></p>')
    raise SystemExit(1)
print('<output style=" height:60px; width:300; background-color:white; border-radius: 7px; padding-top: 10px; padding-bottom: 10px; padding-left: 30px; padding-right: 30px; font-size:24px; font-weight:bold" >{0} * {1} = {2}</output>'.format(num1, num2, num1 * num2))
print('<p><br/><br/><a href="../../index.html">Back to homepage</a></p>')
print('</div')
print('</body>')
