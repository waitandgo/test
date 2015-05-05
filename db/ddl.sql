create table Directory
(
    Telephone varchar(250),
    Type varchar(250),									
    Comment varchar(250)							
)

select Telephone, Type, Comment from Directory;

select * from Directory;
insert into Directory values ('+375 (29) 345-44-55', 'Мобильный', 'Велком');
insert into Directory values ('+375 (29) 545-44-55', 'Мобильный', 'МТС');
insert into Directory values ('+375 (29) 545-44-55', 'Городской', 'Белтелеком');


create table DirectoryPort
(
    Port varchar(250)						
)


select * from DirectoryPort;
insert into DirectoryPort values ('44404');
insert into DirectoryPort values ('44405');


select * from DirectoryPort order by 1 desc;