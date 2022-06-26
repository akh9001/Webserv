
import cgi, cgitb              
  
form = cgi.FieldStorage()      
your_name = form.getvalue('your_name')    

company_name = form.getvalue('company_name')   
  
print ("Content-type:text/html\n")
print ("<html>")
print ("<head>")
print ("<title>First CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello, {} is working in {} </h2>".format(your_name, company_name))
# while 1:
# 	continue

print ("</body>")
print ("</html>")