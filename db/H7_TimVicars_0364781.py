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
select distinct CONCAT(s.Fname," ",s.Lname) as SName, count(c.ct_id),
    group_concat(concat(ct.Fname," ",ct.LName,if(ct.ct_id=s.Main_CT_ID,'(p)','(o)')))
from Swimmer s
    join Participation p on s.SwimmerID = p.SwimmerId and p.EventId=%s
    join (select swimmerid, main_ct_id as ct_id from Swimmer
    union select swimmerid, ct_id from OtherCareTaker) as c
        on s.SwimmerID = c.SwimmerID
    join Caretaker ct on ct.CT_ID = c.ct_id
    group by SName;
'''

eventid = sys.argv[1]
cursor.execute(query, (eventid))
print('Swimmers in Event #' + eventid)
print('------------------')
for (swimmer,ncaretakers,caretakers) in cursor:
    print(' ' + str(swimmer) + ': ' + str(ncaretakers) + ' caretakers =>' + caretakers)

cursor.close()
cnx.close()							  
