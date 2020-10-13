from dbconfig import *
import pymysql
import cgi
import cgitb
cgitb.enable()

#	Establish a cursor for MySQL connection.
db = get_mysql_param()
cnx = pymysql.connect(user=db['user'], 
                      password=db['password'],
                      host=db['host'],
                      database=db['database'])
                              
cursor = cnx.cursor()

#	Create HTTP response header
print("Content-Type: text/html;charset=utf-8")
print()

#	Create a primitive HTML starter
print ('''<html>
<head></head>
<body>
''')

#	Get HTTP parameter, ctid (caretaker id) and sid (swimmer id)
form = cgi.FieldStorage()
ctid = form.getfirst('ctid')
sid = form.getfirst('sid')

if ctid is None and sid is None:
	#	No HTTP parameter: show all caretakers for selection.
	print('<h3>Please select the caretaker:</h3>')
	print('<ol>')

	query = '''
select distinct c.ct_id, c.lname, c.fname
from caretaker c
order by c.lname, c.fname;
'''
	cursor.execute(query)
	for (ctid, lname, fname) in cursor:
		print('    <li><a href="h10.py?ctid=' + str(ctid) + '">'
			+ fname + ' ' + lname + '</a></li>')
	
	print('</ol>')
	print('</body></html>')
	cursor.close()
	cnx.close()		
	quit()
	
if ctid is not None:
	#	Show the swimmers under the caretaker.
	#	Alternative and simpler solutions with two queries are possible.
	
	query = '''
select distinct c.CT_Id,
	concat(c.FName, ' ', c.LName) as caretaker,
	group_concat(concat('   <li><a href="?sid=', s.SwimmerId, '"</a>',
		s.FName, ' ', s.LName, '</li>\n') separator '') as swimmers
from caretaker c left join swimmer s on (s.Main_CT_Id = c.CT_Id)
group by caretaker
having c.CT_Id = %s;
'''
	cursor.execute(query,(int(ctid),))
	(ctid_out, caretaker, swimmers) = cursor.fetchone()
	if swimmers is not None:
		print('<h3>Swimmers under primary care of ' + caretaker + ':</h3>')
		print('<ol>')
		print(swimmers)
		print('</ol>')				  
	else:
		print('<h3>There is no swimmer under primary care of ' + caretaker + '</h3>')

	print ('''</body>
</html>''')
	cursor.close()
	cnx.close()		
	quit()
	
if sid is not None:	#	This will always be satisfied at this point.
	query = '''
select distinct s.lName, s.fName, l.level
from Swimmer s, Level l
where s.CurrentLevelId = l.LevelId
and s.SwimmerId = %s
'''

	cursor.execute(query,(int(sid),))
	(lname, fname, level) = cursor.fetchone()
	
	print('<p>Swimmer #' + str(sid) + ' ' +
			 fname + ' ' + lname + ': currnet level: ' + level)
                  
cursor.close()
cnx.close()		
				  
print ('''</body>
</html>''')