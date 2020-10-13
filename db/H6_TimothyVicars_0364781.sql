#question 1
select lname, fname, phone, currentlevelid from Swimmer 
where currentlevelid >=3;

#question 2
select s.LName, s.Fname, l.level from Swimmer s join Level l 
where l.levelid=s.CurrentLevelId and s.Lname='Khan';

#question 3
select s.Lname, s.Fname from Swimmer s join Participation p join Event e join Meet m
where p.SwimmerId=s.SwimmerId
and p.EventId=e.EventId
and e.Title='50M Butterfly'
and m.Title='UHCL Open'
and m.MeetId=e.MeetId;

# question 4
create table R4 as select swimmerid, main_ct_id as ct_id from Swimmer
union select swimmerid, ct_id from OtherCareTaker;
select * from R4;

# question 5
select c.lname,c.fname from R4 r 
join R4 r2 on r.ct_id=r2.ct_id and r2.swimmerid=1 and r.swimmerid=2
join Caretaker c on r.ct_id=c.ct_id;
drop table R4;

# question 6
select m.meetid,m.title,COUNT(*) as 'Number of butterfly events' from Meet m
join Event e where e.MeetId=m.meetid and e.Title like '%Butterfly%'
group by m.meetid;

# question 7
select distinct CONCAT(s.Fname,' ',s.Lname) as student, l.level 
from Participation p 
join Swimmer s
join Event e on e.EventId=p.EventId
join Level l on l.LevelId=e.LevelId
where s.swimmerid not in (select swimmerid from Participation where Eventid=5)
and e.EventId=3;

# question 8
create table R4 as select swimmerid, main_ct_id as ct_id from Swimmer
union select swimmerid, ct_id from OtherCareTaker;
select concat(s.fname,' ',s.lname) as swimmer,l.level, count(*) as 'Number of Caretakers'
from Swimmer s
join R4 r on s.swimmerid=r.swimmerid
join Level l on s.CurrentLevelId = l.LevelId
group by s.swimmerid having count(*) > 1
order by count(*) desc, s.swimmerid desc;
drop table R4;
