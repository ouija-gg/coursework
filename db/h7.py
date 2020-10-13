#!/usr/bin/python
#list all swimmers participated in the event with the event id eid 
#in the following manner. 
#For every swimmer, provide the swimmer's names. number of 
#caretakers and the names of the caretakers. A '(p)' follows a 
#primary caretaker and a '(o)' follows an other caretaker.

from dbconfig import *
import pymysql
import sys, getopt

db = get_mysql_param()
cnx = pymysql.connect(user=db['user'], password=db['password'],
                      host=db['host'],
                      database=db['database'])
cursor = cnx.cursor()
query = '''
select distinct e.Title,
    count(distinct p.SwimmerId) as numSwimmers,
    group_concat(concat(s.fname, ' ', s.lname, 
        if(p.comment is null, '', concat(': ', p.comment)))
order by s.lname, s.fname
separator '; ') as result
from Event e, Participation p, Swimmer s
where e.EventId = p.EventId
    and p.SwimmerId = s.SwimmerId
    and e.MeetId = %s
group by e.Title;
'''
query2 = '''
select distinct CONCAT(s.Fname," ",s.Lname) as SName from Swimmer s;
'''

meetId = sys.argv[1]
cursor.execute(query, (meetId,))
#cursor.execute(query)
print('Events in Meet #' + meetId)
print('------------------')

for (event, nSwimmers, result) in cursor:
    print('  ' + event + ': '
        + str(nSwimmers) + ' swimmers> '
        + result)
cursor.close()
cnx.close()							  
